
// RUN: %clang_cc1 %s -fopenacc -verify

typedef struct IsComplete {
  struct S { int A; } CompositeMember;
  int ScalarMember;
  float ArrayMember[5];
  void *PointerMember;
} Complete;
void uses(int IntParam, short *PointerParam, float ArrayParam[5], Complete CompositeParam) {
  int LocalInt;
  short *LocalPointer;
  float LocalArray[5];
  Complete LocalComposite;
  // Check Appertainment:
#pragma acc parallel create(LocalInt)
  while(1);
#pragma acc serial create(LocalInt)
  while(1);
#pragma acc kernels create(LocalInt)
  while(1);

  // expected-warning@+1{{OpenACC clause name 'pcreate' is a deprecated clause name and is now an alias for 'create'}}
#pragma acc parallel pcreate(LocalInt)
  while(1);

  // expected-warning@+1{{OpenACC clause name 'present_or_create' is a deprecated clause name and is now an alias for 'create'}}
#pragma acc parallel present_or_create(LocalInt)
  while(1);

  // Valid cases:
#pragma acc parallel create(LocalInt, LocalPointer, LocalArray)
  while(1);
#pragma acc parallel create(LocalArray[2:1])
  while(1);
#pragma acc parallel create(zero:LocalArray[2:1])
  while(1);

#pragma acc parallel create(LocalComposite.ScalarMember, LocalComposite.ScalarMember)
  while(1);

  // expected-error@+1{{OpenACC variable is not a valid variable name, sub-array, array element, member of a composite variable, or composite variable member}}
#pragma acc parallel create(1 + IntParam)
  while(1);

  // expected-error@+1{{OpenACC variable is not a valid variable name, sub-array, array element, member of a composite variable, or composite variable member}}
#pragma acc parallel create(+IntParam)
  while(1);

  // expected-error@+1{{OpenACC sub-array length is unspecified and cannot be inferred because the subscripted value is not an array}}
#pragma acc parallel create(PointerParam[2:])
  while(1);

  // expected-error@+1{{OpenACC sub-array specified range [2:5] would be out of the range of the subscripted array size of 5}}
#pragma acc parallel create(ArrayParam[2:5])
  while(1);

  // expected-error@+2{{OpenACC sub-array specified range [2:5] would be out of the range of the subscripted array size of 5}}
  // expected-error@+1{{OpenACC variable is not a valid variable name, sub-array, array element, member of a composite variable, or composite variable member}}
#pragma acc parallel create((float*)ArrayParam[2:5])
  while(1);
  // expected-error@+1{{OpenACC variable is not a valid variable name, sub-array, array element, member of a composite variable, or composite variable member}}
#pragma acc parallel create((float)ArrayParam[2])
  while(1);
  // expected-error@+2{{unknown modifier 'invalid' in OpenACC modifier-list on 'create' clause}}
  // expected-error@+1{{OpenACC variable is not a valid variable name, sub-array, array element, member of a composite variable, or composite variable member}}
#pragma acc parallel create(invalid:(float)ArrayParam[2])
  while(1);

  // expected-error@+1{{OpenACC 'create' clause is not valid on 'loop' directive}}
#pragma acc loop create(LocalInt)
  for(int i = 5; i < 10;++i);
  // expected-error@+1{{OpenACC 'pcreate' clause is not valid on 'loop' directive}}
#pragma acc loop pcreate(LocalInt)
  for(int i = 5; i < 10;++i);
  // expected-error@+1{{OpenACC 'present_or_create' clause is not valid on 'loop' directive}}
#pragma acc loop present_or_create(LocalInt)
  for(int i = 5; i < 10;++i);
}
void ModList() {
  int V1;
  // expected-error@+4{{OpenACC 'always' modifier not valid on 'create' clause}}
  // expected-error@+3{{OpenACC 'alwaysin' modifier not valid on 'create' clause}}
  // expected-error@+2{{OpenACC 'alwaysout' modifier not valid on 'create' clause}}
  // expected-error@+1{{OpenACC 'readonly' modifier not valid on 'create' clause}}
#pragma acc parallel create(always, alwaysin, alwaysout, zero, readonly: V1)
  for(int i = 5; i < 10;++i);
  // expected-error@+1{{OpenACC 'always' modifier not valid on 'create' clause}}
#pragma acc serial create(always: V1)
  for(int i = 5; i < 10;++i);
  // expected-error@+1{{OpenACC 'alwaysin' modifier not valid on 'create' clause}}
#pragma acc kernels create(alwaysin: V1)
  for(int i = 5; i < 10;++i);
  // expected-error@+1{{OpenACC 'alwaysout' modifier not valid on 'create' clause}}
#pragma acc parallel create(alwaysout: V1)
  for(int i = 5; i < 10;++i);
  // expected-error@+1{{OpenACC 'readonly' modifier not valid on 'create' clause}}
#pragma acc serial create(readonly: V1)
  for(int i = 5; i < 10;++i);
#pragma acc parallel create(capture:V1)
  for(int i = 5; i < 10;++i);
}
