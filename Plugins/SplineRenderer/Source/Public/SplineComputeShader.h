// Copyright 2017 Oh-Hyun Kwon. All Rights Reserved.

#pragma once

#include "GlobalShader.h"
#include "RHICommandList.h"
#include "UniformBuffer.h"

// DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(uint32, Degree)
BEGIN_UNIFORM_BUFFER_STRUCT(FSplineComputeShaderUniformParameters, )
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, WorldSize)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(float, Width)
DECLARE_UNIFORM_BUFFER_STRUCT_MEMBER(uint32, NumSides)
END_UNIFORM_BUFFER_STRUCT(FSplineComputeShaderUniformParameters)

typedef TUniformBufferRef<FSplineComputeShaderUniformParameters>
	FSplineComputeShaderUniformParametersRef;

class SPLINERENDERER_API FBaseSplineComputeShader : public FGlobalShader
{
private:
	FShaderResourceParameter InSplineControlPointData;
	FShaderResourceParameter InSplineSegmentData;
	FShaderResourceParameter InSplineData;

	FShaderResourceParameter OutMeshVertexData;

public:
	static bool ShouldCache(EShaderPlatform Platform)
	{
		return RHISupportsComputeShaders(Platform);
	}

	static void ModifyCompilationEnvironment(EShaderPlatform Platform,
											 FShaderCompilerEnvironment& OutEnvironment)
	{
		FGlobalShader::ModifyCompilationEnvironment(Platform, OutEnvironment);
		OutEnvironment.SetDefine(TEXT("SPLINE_COMPUTE_SHADER_MAX_SAMPLES_PER_SEGMENT"), 64);
	}

	FBaseSplineComputeShader()
	{
	}

	explicit FBaseSplineComputeShader(
		ShaderMetaType::CompiledShaderInitializerType const& Initializer);

	virtual bool Serialize(FArchive& Ar) override
	{
		bool bShaderHasOutdatedParams = FGlobalShader::Serialize(Ar);
		Ar << InSplineControlPointData;
		Ar << InSplineSegmentData;
		Ar << InSplineData;
		Ar << OutMeshVertexData;
		return bShaderHasOutdatedParams;
	}

	void SetUniformBuffers(FRHICommandList& RHICmdList,
						   FSplineComputeShaderUniformParameters& ConstantParameters);

	void SetBuffers(FRHICommandList& RHICmdList,
					FShaderResourceViewRHIRef InSplineControlPointBufferSRV,
					FShaderResourceViewRHIRef InSplineSegmentBufferSRV,
					FShaderResourceViewRHIRef InSplineBufferSRV,
					FUnorderedAccessViewRHIRef OutMeshVertexBufferUAV);

	void UnbindBuffers(FRHICommandList& RHICmdList);
};

class SPLINERENDERER_API FSplineComputeShader_Sphere : public FBaseSplineComputeShader
{
	DECLARE_SHADER_TYPE(FSplineComputeShader_Sphere, Global);

public:
	FSplineComputeShader_Sphere()
	{
	}

	explicit FSplineComputeShader_Sphere(
		ShaderMetaType::CompiledShaderInitializerType const& Initializer);
};
