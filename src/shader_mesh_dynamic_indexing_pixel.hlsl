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

struct PSInput
{
	float4 position    : SV_POSITION;
	float3 normal	 : NORMAL;
	float2 uv        : TEXCOORD0;
	float3 outVec : POSITION0;
	float4 vizColor : COLOR0;

};

struct MaterialConstants
{
    uint matIndex;    // Dynamically set index for looking up from g_txMats[].
};

ConstantBuffer<MaterialConstants> materialConstants : register(b0, space0);
Texture2D        g_txDiffuse    : register(t0);
Texture2D        g_txMats[]    : register(t1);
SamplerState    g_sampler    : register(s0);

float4 PSMain(PSInput input) : SV_TARGET
{
   float4 diffuse = g_txDiffuse.Sample(g_sampler, input.uv).rgba;
   float4 mat = g_txMats[materialConstants.matIndex].Sample(g_sampler, input.uv).rgba;


	mat.a = input.vizColor.a;
	diffuse = diffuse * float4(input.vizColor.rgb,1.0f);
	if (mat.a != 0.0f)
		diffuse = diffuse * mat;


   // Artificially large test case
	for (int i = 0; i < 100000; i++) {
	   diffuse -= float4(0.1f, 0.1f, 0.1f, 0.0f);
	   diffuse += float4(0.1f, 0.1f, 0.1f, 0.0f);
	}

   return float4(diffuse.rgb, 1.0f);

}