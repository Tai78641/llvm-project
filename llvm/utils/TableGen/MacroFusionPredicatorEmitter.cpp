//===------ MacroFusionPredicatorEmitter.cpp - Generator for Fusion ------===//
//
// Part of the LLVM Project, under the Apache License v2.0 with LLVM Exceptions.
// See https://llvm.org/LICENSE.txt for license information.
// SPDX-License-Identifier: Apache-2.0 WITH LLVM-exception
//
//===---------------------------------------------------------------------===//
//
// MacroFusionPredicatorEmitter implements a TableGen-driven predicators
// generator for macro-op fusions.
//
// This TableGen backend processes `Fusion` definitions and generates
// predicators for checking if input instructions can be fused. These
// predicators can used in `MacroFusion` DAG mutation.
//
// The generated header file contains two parts: one for predicator
// declarations and one for predicator implementations. The user can get them
// by defining macro `GET_<TargetName>_MACRO_FUSION_PRED_DECL` or
// `GET_<TargetName>_MACRO_FUSION_PRED_IMPL` and then including the generated
// header file.
//
// The generated predicator will be like:
//
// ```
// bool isNAME(const TargetInstrInfo &TII,
//             const TargetSubtargetInfo &STI,
//             const MachineInstr *FirstMI,
//             const MachineInstr &SecondMI) {
//   auto &MRI = SecondMI.getMF()->getRegInfo();
//   /* Predicates */
//   return true;
// }
// ```
//
// The `Predicates` part is generated from a list of `FusionPredicate`, which
// can be predefined predicates, a raw code string or `MCInstPredicate` defined
// in TargetInstrPredicate.td.
//
//===---------------------------------------------------------------------===//

#include "Common/CodeGenTarget.h"
#include "Common/PredicateExpander.h"
#include "llvm/Support/Debug.h"
#include "llvm/TableGen/Error.h"
#include "llvm/TableGen/Record.h"
#include "llvm/TableGen/TableGenBackend.h"
#include <vector>

using namespace llvm;

#define DEBUG_TYPE "macro-fusion-predicator"

namespace {
class MacroFusionPredicatorEmitter {
  const RecordKeeper &Records;
  const CodeGenTarget Target;

  void emitMacroFusionDecl(ArrayRef<const Record *> Fusions,
                           PredicateExpander &PE, raw_ostream &OS);
  void emitMacroFusionImpl(ArrayRef<const Record *> Fusions,
                           PredicateExpander &PE, raw_ostream &OS);
  void emitPredicates(ArrayRef<const Record *> FirstPredicate,
                      bool IsCommutable, PredicateExpander &PE,
                      raw_ostream &OS);
  void emitFirstPredicate(const Record *SecondPredicate, bool IsCommutable,
                          PredicateExpander &PE, raw_ostream &OS);
  void emitSecondPredicate(const Record *SecondPredicate, bool IsCommutable,
                           PredicateExpander &PE, raw_ostream &OS);
  void emitBothPredicate(const Record *Predicates, bool IsCommutable,
                         PredicateExpander &PE, raw_ostream &OS);

public:
  MacroFusionPredicatorEmitter(const RecordKeeper &R) : Records(R), Target(R) {}

  void run(raw_ostream &OS);
};
} // End anonymous namespace.

void MacroFusionPredicatorEmitter::emitMacroFusionDecl(
    ArrayRef<const Record *> Fusions, PredicateExpander &PE, raw_ostream &OS) {
  OS << "#ifdef GET_" << Target.getName() << "_MACRO_FUSION_PRED_DECL\n";
  OS << "#undef GET_" << Target.getName() << "_MACRO_FUSION_PRED_DECL\n\n";
  OS << "namespace llvm {\n";

  for (const Record *Fusion : Fusions) {
    OS << "bool is" << Fusion->getName() << "(const TargetInstrInfo &, "
       << "const TargetSubtargetInfo &, "
       << "const MachineInstr *, "
       << "const MachineInstr &);\n";
  }

  OS << "} // end namespace llvm\n";
  OS << "\n#endif\n";
}

void MacroFusionPredicatorEmitter::emitMacroFusionImpl(
    ArrayRef<const Record *> Fusions, PredicateExpander &PE, raw_ostream &OS) {
  OS << "#ifdef GET_" << Target.getName() << "_MACRO_FUSION_PRED_IMPL\n";
  OS << "#undef GET_" << Target.getName() << "_MACRO_FUSION_PRED_IMPL\n\n";
  OS << "namespace llvm {\n";

  for (const Record *Fusion : Fusions) {
    std::vector<const Record *> Predicates =
        Fusion->getValueAsListOfDefs("Predicates");
    bool IsCommutable = Fusion->getValueAsBit("IsCommutable");

    OS << "bool is" << Fusion->getName() << "(\n";
    OS.indent(4) << "const TargetInstrInfo &TII,\n";
    OS.indent(4) << "const TargetSubtargetInfo &STI,\n";
    OS.indent(4) << "const MachineInstr *FirstMI,\n";
    OS.indent(4) << "const MachineInstr &SecondMI) {\n";
    OS.indent(2)
        << "[[maybe_unused]] auto &MRI = SecondMI.getMF()->getRegInfo();\n";

    emitPredicates(Predicates, IsCommutable, PE, OS);

    OS.indent(2) << "return true;\n";
    OS << "}\n";
  }

  OS << "} // end namespace llvm\n";
  OS << "\n#endif\n";
}

void MacroFusionPredicatorEmitter::emitPredicates(
    ArrayRef<const Record *> Predicates, bool IsCommutable,
    PredicateExpander &PE, raw_ostream &OS) {
  for (const Record *Predicate : Predicates) {
    const Record *Target = Predicate->getValueAsDef("Target");
    if (Target->getName() == "first_fusion_target")
      emitFirstPredicate(Predicate, IsCommutable, PE, OS);
    else if (Target->getName() == "second_fusion_target")
      emitSecondPredicate(Predicate, IsCommutable, PE, OS);
    else if (Target->getName() == "both_fusion_target")
      emitBothPredicate(Predicate, IsCommutable, PE, OS);
    else
      PrintFatalError(Target->getLoc(),
                      "Unsupported 'FusionTarget': " + Target->getName());
  }
}

void MacroFusionPredicatorEmitter::emitFirstPredicate(const Record *Predicate,
                                                      bool IsCommutable,
                                                      PredicateExpander &PE,
                                                      raw_ostream &OS) {
  if (Predicate->isSubClassOf("WildcardPred")) {
    OS.indent(2) << "if (!FirstMI)\n";
    OS.indent(2) << "  return "
                 << (Predicate->getValueAsBit("ReturnValue") ? "true" : "false")
                 << ";\n";
  } else if (Predicate->isSubClassOf("OneUsePred")) {
    OS.indent(2) << "{\n";
    OS.indent(4) << "Register FirstDest = FirstMI->getOperand(0).getReg();\n";
    OS.indent(4)
        << "if (FirstDest.isVirtual() && !MRI.hasOneNonDBGUse(FirstDest))\n";
    OS.indent(4) << "  return false;\n";
    OS.indent(2) << "}\n";
  } else if (Predicate->isSubClassOf("FirstInstHasSameReg")) {
    int FirstOpIdx = Predicate->getValueAsInt("FirstOpIdx");
    int SecondOpIdx = Predicate->getValueAsInt("SecondOpIdx");

    OS.indent(2) << "if (!FirstMI->getOperand(" << FirstOpIdx
                 << ").getReg().isVirtual()) {\n";
    OS.indent(4) << "if (FirstMI->getOperand(" << FirstOpIdx
                 << ").getReg() != FirstMI->getOperand(" << SecondOpIdx
                 << ").getReg())";

    if (IsCommutable) {
      OS << " {\n";
      OS.indent(6) << "if (!FirstMI->getDesc().isCommutable())\n";
      OS.indent(6) << "  return false;\n";

      OS.indent(6)
          << "unsigned SrcOpIdx1 = " << SecondOpIdx
          << ", SrcOpIdx2 = TargetInstrInfo::CommuteAnyOperandIndex;\n";
      OS.indent(6)
          << "if (TII.findCommutedOpIndices(FirstMI, SrcOpIdx1, SrcOpIdx2))\n";
      OS.indent(6)
          << "  if (FirstMI->getOperand(" << FirstOpIdx
          << ").getReg() != FirstMI->getOperand(SrcOpIdx2).getReg())\n";
      OS.indent(6) << "    return false;\n";
      OS.indent(4) << "}\n";
    } else {
      OS << "\n";
      OS.indent(4) << "  return false;\n";
    }
    OS.indent(2) << "}\n";
  } else if (Predicate->isSubClassOf("FusionPredicateWithMCInstPredicate")) {
    OS.indent(2) << "{\n";
    OS.indent(4) << "const MachineInstr *MI = FirstMI;\n";
    OS.indent(4) << "if (";
    PE.setNegatePredicate(true);
    PE.getIndent() = 3;
    PE.expandPredicate(OS, Predicate->getValueAsDef("Predicate"));
    OS << ")\n";
    OS.indent(4) << "  return false;\n";
    OS.indent(2) << "}\n";
  } else {
    PrintFatalError(Predicate->getLoc(),
                    "Unsupported predicate for first instruction: " +
                        Predicate->getType()->getAsString());
  }
}

void MacroFusionPredicatorEmitter::emitSecondPredicate(const Record *Predicate,
                                                       bool IsCommutable,
                                                       PredicateExpander &PE,
                                                       raw_ostream &OS) {
  if (Predicate->isSubClassOf("FusionPredicateWithMCInstPredicate")) {
    OS.indent(2) << "{\n";
    OS.indent(4) << "const MachineInstr *MI = &SecondMI;\n";
    OS.indent(4) << "if (";
    PE.setNegatePredicate(true);
    PE.getIndent() = 3;
    PE.expandPredicate(OS, Predicate->getValueAsDef("Predicate"));
    OS << ")\n";
    OS.indent(4) << "  return false;\n";
    OS.indent(2) << "}\n";
  } else if (Predicate->isSubClassOf("SecondInstHasSameReg")) {
    int FirstOpIdx = Predicate->getValueAsInt("FirstOpIdx");
    int SecondOpIdx = Predicate->getValueAsInt("SecondOpIdx");

    OS.indent(2) << "if (!SecondMI.getOperand(" << FirstOpIdx
                 << ").getReg().isVirtual()) {\n";
    OS.indent(4) << "if (SecondMI.getOperand(" << FirstOpIdx
                 << ").getReg() != SecondMI.getOperand(" << SecondOpIdx
                 << ").getReg())";

    if (IsCommutable) {
      OS << " {\n";
      OS.indent(6) << "if (!SecondMI.getDesc().isCommutable())\n";
      OS.indent(6) << "  return false;\n";

      OS.indent(6)
          << "unsigned SrcOpIdx1 = " << SecondOpIdx
          << ", SrcOpIdx2 = TargetInstrInfo::CommuteAnyOperandIndex;\n";
      OS.indent(6)
          << "if (TII.findCommutedOpIndices(SecondMI, SrcOpIdx1, SrcOpIdx2))\n";
      OS.indent(6)
          << "  if (SecondMI.getOperand(" << FirstOpIdx
          << ").getReg() != SecondMI.getOperand(SrcOpIdx2).getReg())\n";
      OS.indent(6) << "    return false;\n";
      OS.indent(4) << "}\n";
    } else {
      OS << "\n";
      OS.indent(4) << "  return false;\n";
    }
    OS.indent(2) << "}\n";
  } else {
    PrintFatalError(Predicate->getLoc(),
                    "Unsupported predicate for second instruction: " +
                        Predicate->getType()->getAsString());
  }
}

void MacroFusionPredicatorEmitter::emitBothPredicate(const Record *Predicate,
                                                     bool IsCommutable,
                                                     PredicateExpander &PE,
                                                     raw_ostream &OS) {
  if (Predicate->isSubClassOf("FusionPredicateWithCode"))
    OS << Predicate->getValueAsString("Predicate");
  else if (Predicate->isSubClassOf("BothFusionPredicateWithMCInstPredicate")) {
    emitFirstPredicate(Predicate, IsCommutable, PE, OS);
    emitSecondPredicate(Predicate, IsCommutable, PE, OS);
  } else if (Predicate->isSubClassOf("TieReg")) {
    int FirstOpIdx = Predicate->getValueAsInt("FirstOpIdx");
    int SecondOpIdx = Predicate->getValueAsInt("SecondOpIdx");
    OS.indent(2) << "if (!(FirstMI->getOperand(" << FirstOpIdx
                 << ").isReg() &&\n";
    OS.indent(2) << "      SecondMI.getOperand(" << SecondOpIdx
                 << ").isReg() &&\n";
    OS.indent(2) << "      FirstMI->getOperand(" << FirstOpIdx
                 << ").getReg() == SecondMI.getOperand(" << SecondOpIdx
                 << ").getReg()))";

    if (IsCommutable) {
      OS << " {\n";
      OS.indent(4) << "if (!SecondMI.getDesc().isCommutable())\n";
      OS.indent(4) << "  return false;\n";

      OS.indent(4)
          << "unsigned SrcOpIdx1 = " << SecondOpIdx
          << ", SrcOpIdx2 = TargetInstrInfo::CommuteAnyOperandIndex;\n";
      OS.indent(4)
          << "if (TII.findCommutedOpIndices(SecondMI, SrcOpIdx1, SrcOpIdx2))\n";
      OS.indent(4)
          << "  if (FirstMI->getOperand(" << FirstOpIdx
          << ").getReg() != SecondMI.getOperand(SrcOpIdx2).getReg())\n";
      OS.indent(4) << "    return false;\n";
      OS.indent(2) << "}";
    } else {
      OS << "\n";
      OS.indent(2) << "  return false;";
    }
    OS << "\n";
  } else {
    PrintFatalError(Predicate->getLoc(),
                    "Unsupported predicate for both instruction: " +
                        Predicate->getType()->getAsString());
  }
}

void MacroFusionPredicatorEmitter::run(raw_ostream &OS) {
  // Emit file header.
  emitSourceFileHeader("Macro Fusion Predicators", OS);

  PredicateExpander PE(Target.getName());
  PE.setByRef(false);
  PE.setExpandForMC(false);

  ArrayRef<const Record *> Fusions = Records.getAllDerivedDefinitions("Fusion");
  emitMacroFusionDecl(Fusions, PE, OS);
  OS << "\n";
  emitMacroFusionImpl(Fusions, PE, OS);
}

static TableGen::Emitter::OptClass<MacroFusionPredicatorEmitter>
    X("gen-macro-fusion-pred", "Generate macro fusion predicators.");
