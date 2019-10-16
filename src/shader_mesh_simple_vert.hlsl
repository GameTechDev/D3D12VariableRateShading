//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

//*********************************************************
//
// Copyright 2019 Intel Corporation 
//
// Permission is hereby granted, free of charge, to any 
// person obtaining a copy of this software and associated 
// documentation files(the "Software"), to deal in the Software 
// without restriction, including without limitation the rights 
// to use, copy, modify, merge, publish, distribute, sublicense, 
// and/or sell copies of the Software, and to permit persons to 
// whom the Software is furnished to do so, subject to the 
// following conditions :
// The above copyright notice and this permission notice shall 
// be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, 
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF 
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT.IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT 
// HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, 
// WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER 
// DEALINGS IN THE SOFTWARE.
//
//*********************************************************

struct VSInput
{
    float3 position    : POSITION;
    float3 normal    : NORMAL;
    float2 uv        : TEXCOORD0;
    float3 tangent    : TANGENT;
};

struct PSInput
{
    float4 position    : SV_POSITION;
	float3 normal	 : NORMAL;
    float2 uv        : TEXCOORD0;
	float3 outVec : POSITION0;
	float4 vizColor : COLOR0;
};

cbuffer cb0 : register(b0)
{
    float4x4 g_mWorldViewProj;
	float4 offset;
	int shadingRate;
	float enableViz;
};

PSInput VSMain(VSInput input)
{
    PSInput result;
    
    result.position = mul(float4(input.position, 1.0f) + float4(offset.x,1.0f,1.0f,1.0f), g_mWorldViewProj);
	result.normal = input.normal;
    result.uv = input.uv;
	
	result.vizColor = float4(1.0f, 1.0f, 1.0f, 1.0f);

	if (enableViz == 0)
		result.vizColor = float4(1.0f, 1.0f, 1.0f, 1.0f);
	else if (shadingRate == 0)
		result.vizColor = float4(0.0f, 0.0f, 1.0f, 0.0f);
	else if (shadingRate == 1)
		result.vizColor = float4(0.0f, 0.5f, 0.5f, 0.0f);
	else if (shadingRate == 4)
		result.vizColor = float4(0.5f, 0.5f, 0.0f, 0.0f);
	else if (shadingRate == 5)
		result.vizColor = float4(0.0f, 1.0f, 0.0f, 0.0f);
	else if (shadingRate == 10)
		result.vizColor = float4(1.0f, 0.0f, 0.0f, 0.0f);

	float4 eye = float4(0.0f, 0.0f, -2.0f, 1.0f);
	result.outVec = -(eye.xyz - result.normal);

    
    return result;
}
