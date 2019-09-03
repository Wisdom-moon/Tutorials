; ModuleID = 'vecadd.cpp'
source_filename = "vecadd.cpp"
target datalayout = "e-m:e-i64:64-f80:128-n8:16:32:64-S128"
target triple = "x86_64-unknown-linux-gnu"

@.str = private unnamed_addr constant [5 x i8] c"%f  \00", align 1
@.str.1 = private unnamed_addr constant [2 x i8] c"\0A\00", align 1

; Function Attrs: noinline norecurse optnone uwtable
define i32 @main(i32 %argc, i8** %argv) #0 {
entry:
  %retval = alloca i32, align 4
  %argc.addr = alloca i32, align 4
  %argv.addr = alloca i8**, align 8
  %inputLength = alloca i32, align 4
  %inputLengthBytes = alloca i32, align 4
  %hostInput1 = alloca float*, align 8
  %hostInput2 = alloca float*, align 8
  %hostOutput = alloca float*, align 8
  %i = alloca i32, align 4
  %i15 = alloca i32, align 4
  %i28 = alloca i32, align 4
  store i32 0, i32* %retval, align 4
  store i32 %argc, i32* %argc.addr, align 4
  store i8** %argv, i8*** %argv.addr, align 8
  store i32 32, i32* %inputLength, align 4
  %0 = load i32, i32* %argc.addr, align 4
  %cmp = icmp sgt i32 %0, 1
  br i1 %cmp, label %if.then, label %if.end

if.then:                                          ; preds = %entry
  %1 = load i8**, i8*** %argv.addr, align 8
  %arrayidx = getelementptr inbounds i8*, i8** %1, i64 1
  %2 = load i8*, i8** %arrayidx, align 8
  %call = call i32 @atoi(i8* %2) #4
  store i32 %call, i32* %inputLength, align 4
  br label %if.end

if.end:                                           ; preds = %if.then, %entry
  %3 = load i32, i32* %inputLength, align 4
  %conv = sext i32 %3 to i64
  %mul = mul i64 %conv, 4
  %conv1 = trunc i64 %mul to i32
  store i32 %conv1, i32* %inputLengthBytes, align 4
  %4 = load i32, i32* %inputLengthBytes, align 4
  %conv2 = sext i32 %4 to i64
  %call3 = call noalias i8* @malloc(i64 %conv2) #5
  %5 = bitcast i8* %call3 to float*
  store float* %5, float** %hostInput1, align 8
  %6 = load i32, i32* %inputLengthBytes, align 4
  %conv4 = sext i32 %6 to i64
  %call5 = call noalias i8* @malloc(i64 %conv4) #5
  %7 = bitcast i8* %call5 to float*
  store float* %7, float** %hostInput2, align 8
  %8 = load i32, i32* %inputLengthBytes, align 4
  %conv6 = sext i32 %8 to i64
  %call7 = call noalias i8* @malloc(i64 %conv6) #5
  %9 = bitcast i8* %call7 to float*
  store float* %9, float** %hostOutput, align 8
  store i32 0, i32* %i, align 4
  br label %for.cond

for.cond:                                         ; preds = %for.inc, %if.end
  %10 = load i32, i32* %i, align 4
  %11 = load i32, i32* %inputLength, align 4
  %cmp8 = icmp slt i32 %10, %11
  br i1 %cmp8, label %for.body, label %for.end

for.body:                                         ; preds = %for.cond
  %12 = load i32, i32* %i, align 4
  %rem = srem i32 %12, 11
  %conv9 = sitofp i32 %rem to float
  %13 = load float*, float** %hostInput1, align 8
  %14 = load i32, i32* %i, align 4
  %idxprom = sext i32 %14 to i64
  %arrayidx10 = getelementptr inbounds float, float* %13, i64 %idxprom
  store float %conv9, float* %arrayidx10, align 4
  %15 = load i32, i32* %i, align 4
  %rem11 = srem i32 %15, 31
  %conv12 = sitofp i32 %rem11 to float
  %16 = load float*, float** %hostInput2, align 8
  %17 = load i32, i32* %i, align 4
  %idxprom13 = sext i32 %17 to i64
  %arrayidx14 = getelementptr inbounds float, float* %16, i64 %idxprom13
  store float %conv12, float* %arrayidx14, align 4
  br label %for.inc

for.inc:                                          ; preds = %for.body
  %18 = load i32, i32* %i, align 4
  %inc = add nsw i32 %18, 1
  store i32 %inc, i32* %i, align 4
  br label %for.cond

for.end:                                          ; preds = %for.cond
  store i32 0, i32* %i15, align 4
  br label %for.cond16

for.cond16:                                       ; preds = %for.inc25, %for.end
  %19 = load i32, i32* %i15, align 4
  %20 = load i32, i32* %inputLength, align 4
  %cmp17 = icmp slt i32 %19, %20
  br i1 %cmp17, label %for.body18, label %for.end27

for.body18:                                       ; preds = %for.cond16
  %21 = load float*, float** %hostInput1, align 8
  %22 = load i32, i32* %i15, align 4
  %idxprom19 = sext i32 %22 to i64
  %arrayidx20 = getelementptr inbounds float, float* %21, i64 %idxprom19
  %23 = load float, float* %arrayidx20, align 4
  %24 = load float*, float** %hostInput2, align 8
  %25 = load i32, i32* %i15, align 4
  %idxprom21 = sext i32 %25 to i64
  %arrayidx22 = getelementptr inbounds float, float* %24, i64 %idxprom21
  %26 = load float, float* %arrayidx22, align 4
  %add = fadd float %23, %26
  %27 = load float*, float** %hostOutput, align 8
  %28 = load i32, i32* %i15, align 4
  %idxprom23 = sext i32 %28 to i64
  %arrayidx24 = getelementptr inbounds float, float* %27, i64 %idxprom23
  store float %add, float* %arrayidx24, align 4
  br label %for.inc25

for.inc25:                                        ; preds = %for.body18
  %29 = load i32, i32* %i15, align 4
  %inc26 = add nsw i32 %29, 1
  store i32 %inc26, i32* %i15, align 4
  br label %for.cond16

for.end27:                                        ; preds = %for.cond16
  store i32 0, i32* %i28, align 4
  br label %for.cond29

for.cond29:                                       ; preds = %for.inc36, %for.end27
  %30 = load i32, i32* %i28, align 4
  %31 = load i32, i32* %inputLength, align 4
  %cmp30 = icmp slt i32 %30, %31
  br i1 %cmp30, label %for.body31, label %for.end38

for.body31:                                       ; preds = %for.cond29
  %32 = load float*, float** %hostOutput, align 8
  %33 = load i32, i32* %i28, align 4
  %idxprom32 = sext i32 %33 to i64
  %arrayidx33 = getelementptr inbounds float, float* %32, i64 %idxprom32
  %34 = load float, float* %arrayidx33, align 4
  %conv34 = fpext float %34 to double
  %call35 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([5 x i8], [5 x i8]* @.str, i32 0, i32 0), double %conv34)
  br label %for.inc36

for.inc36:                                        ; preds = %for.body31
  %35 = load i32, i32* %i28, align 4
  %inc37 = add nsw i32 %35, 1
  store i32 %inc37, i32* %i28, align 4
  br label %for.cond29

for.end38:                                        ; preds = %for.cond29
  %call39 = call i32 (i8*, ...) @printf(i8* getelementptr inbounds ([2 x i8], [2 x i8]* @.str.1, i32 0, i32 0))
  %36 = load float*, float** %hostInput1, align 8
  %cmp40 = icmp ne float* %36, null
  br i1 %cmp40, label %if.then41, label %if.end42

if.then41:                                        ; preds = %for.end38
  %37 = load float*, float** %hostInput1, align 8
  %38 = bitcast float* %37 to i8*
  call void @free(i8* %38) #5
  br label %if.end42

if.end42:                                         ; preds = %if.then41, %for.end38
  %39 = load float*, float** %hostInput2, align 8
  %cmp43 = icmp ne float* %39, null
  br i1 %cmp43, label %if.then44, label %if.end45

if.then44:                                        ; preds = %if.end42
  %40 = load float*, float** %hostInput2, align 8
  %41 = bitcast float* %40 to i8*
  call void @free(i8* %41) #5
  br label %if.end45

if.end45:                                         ; preds = %if.then44, %if.end42
  %42 = load float*, float** %hostOutput, align 8
  %cmp46 = icmp ne float* %42, null
  br i1 %cmp46, label %if.then47, label %if.end48

if.then47:                                        ; preds = %if.end45
  %43 = load float*, float** %hostOutput, align 8
  %44 = bitcast float* %43 to i8*
  call void @free(i8* %44) #5
  br label %if.end48

if.end48:                                         ; preds = %if.then47, %if.end45
  ret i32 0
}

; Function Attrs: nounwind readonly
declare i32 @atoi(i8*) #1

; Function Attrs: nounwind
declare noalias i8* @malloc(i64) #2

declare i32 @printf(i8*, ...) #3

; Function Attrs: nounwind
declare void @free(i8*) #2

attributes #0 = { noinline norecurse optnone uwtable "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-jump-tables"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #1 = { nounwind readonly "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #2 = { nounwind "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #3 = { "correctly-rounded-divide-sqrt-fp-math"="false" "disable-tail-calls"="false" "less-precise-fpmad"="false" "no-frame-pointer-elim"="true" "no-frame-pointer-elim-non-leaf" "no-infs-fp-math"="false" "no-nans-fp-math"="false" "no-signed-zeros-fp-math"="false" "no-trapping-math"="false" "stack-protector-buffer-size"="8" "target-cpu"="x86-64" "target-features"="+fxsr,+mmx,+sse,+sse2,+x87" "unsafe-fp-math"="false" "use-soft-float"="false" }
attributes #4 = { nounwind readonly }
attributes #5 = { nounwind }

!llvm.module.flags = !{!0}
!llvm.ident = !{!1}

!0 = !{i32 1, !"wchar_size", i32 4}
!1 = !{!"clang version 5.0.1 (tags/RELEASE_501/final)"}
