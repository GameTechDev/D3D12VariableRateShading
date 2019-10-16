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

#pragma once

#include "DXSample.h"
#include "StepTimer.h"
#include "FrameResource.h"
#include "SimpleCamera.h"
#include <windows.h>

using namespace DirectX;

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

class D3D12VariableRateShading : public DXSample
{
public:
	D3D12VariableRateShading(UINT width, UINT height, std::wstring name, bool bDisableVRS);

    virtual void OnInit();
    virtual void OnUpdate();
    virtual void OnRender();
    virtual void OnDestroy();
    virtual void OnKeyDown(UINT8 key);
    virtual void OnKeyUp(UINT8 key);
	virtual void OnMouseDown(WPARAM btnState, int x, int y);
	virtual void OnMouseMove(WPARAM btnState, int x, int y);

private:
    static const UINT FrameCount = 3;
    static const UINT CityRowCount = 9;
    static const UINT CityColumnCount = 9;
    static const UINT CityMaterialCount = CityRowCount * CityColumnCount;
    static const UINT CityMaterialTextureWidth = 64;
    static const UINT CityMaterialTextureHeight = 64;
    static const UINT CityMaterialTextureChannelCount = 4;
    static const bool UseBundles = false; // Test VRS with bundles
    static const float CitySpacingInterval;

    // Pipeline objects.
    CD3DX12_VIEWPORT m_viewport;
    CD3DX12_RECT m_scissorRect;
    ComPtr<IDXGISwapChain3> m_swapChain;
    ComPtr<ID3D12Device> m_device;
    ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
    ComPtr<ID3D12Resource> m_depthStencil;
    ComPtr<ID3D12CommandAllocator> m_commandAllocator;
    ComPtr<ID3D12CommandQueue> m_commandQueue;
    ComPtr<ID3D12RootSignature> m_rootSignature;
    ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
    ComPtr<ID3D12DescriptorHeap> m_cbvSrvHeap;
    ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
    ComPtr<ID3D12DescriptorHeap> m_samplerHeap;
    ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12QueryHeap> m_gpuTimerQuery = nullptr;
	ComPtr<ID3D12Resource> m_gpuTimerBuffer = nullptr;

#ifdef VRS_ENABLED
	ComPtr<ID3D12PipelineState> m_pipelineStateViz;
	// Must use ID3D12GraphicsCommandList5 for RSSetShadingRate
	ComPtr<ID3D12GraphicsCommandList5> m_commandList;
#else
	ComPtr<ID3D12GraphicsCommandList> m_commandList;
#endif

    // App resources.
    UINT m_numIndices;
    ComPtr<ID3D12Resource> m_vertexBuffer;
    ComPtr<ID3D12Resource> m_indexBuffer;
    ComPtr<ID3D12Resource> m_cityDiffuseTexture;
	ComPtr<ID3D12Resource> m_cityNormalTexture;
    ComPtr<ID3D12Resource> m_cityMaterialTextures[CityMaterialCount];
    D3D12_VERTEX_BUFFER_VIEW m_vertexBufferView;
    D3D12_INDEX_BUFFER_VIEW m_indexBufferView;
    StepTimer m_timer;
    UINT m_cbvSrvDescriptorSize;
    UINT m_rtvDescriptorSize;
    SimpleCamera m_camera;

    // Frame resources.
    std::vector<FrameResource*> m_frameResources;
    FrameResource* m_pCurrentFrameResource;
    UINT m_currentFrameResourceIndex;

    // Synchronization objects.
    UINT m_frameIndex;
    UINT m_frameCounter;
    HANDLE m_fenceEvent;
    ComPtr<ID3D12Fence> m_fence;
    UINT64 m_fenceValue;
	UINT m_currentFrame = 1;

	float m_clearColor[4]{ 0.0f, 0.2f, 0.4f, 1.0f };

    void LoadPipeline();
    void LoadAssets();
	void LoadImGui();
    void CreateFrameResources();
    void PopulateCommandList(FrameResource* pFrameResource);

	// Timer Queries
	void LoadTimer();
	void ReadTimer();
	void DestroyTimer();

	XMFLOAT2 m_framerateOrigin;
	wchar_t m_framerateBuffer[512];


	uint64_t m_startTime = 0;
	uint64_t m_endTime = 0;
	uint64_t m_GPUClocksPerSecond = 0;
	uint64_t *m_timeStampBuffer = nullptr; //just two slots to hold our per frame start and stop time


	double m_totalFrameTime = 0;
	double m_currentFrameTime = 0;
	double m_GPUSecondsPerClock = 0;

	char* m_shadingRates[6]{ "1x1 [2] Blue Viz", "1x2 [3] Teal Viz", "2x1 [4] Yellow Viz",   "2x2 [5] Green Viz",   "4x4 [6] Red Viz" , "Distance based [7]"};

	bool m_showGUI = true;
	bool m_showFPS = true;
	bool m_showMetrics = true;
	bool m_showVRSVisualizer = true;
	bool m_animate = true;
	float m_offsetX = 0.0f;

	D3D12_SHADING_RATE m_shadingRateValues[5]
	{
		D3D12_SHADING_RATE_1X1,
		D3D12_SHADING_RATE_1X2,
		D3D12_SHADING_RATE_2X1,
		D3D12_SHADING_RATE_2X2,
		D3D12_SHADING_RATE_4X4,
	};
	int m_currentShadingRate = D3D12_SHADING_RATE_1X1;

#ifdef VRS_ENABLED
	bool m_enableVRS = true;
	bool m_VRSTier1Enabled = true;
#else
	bool m_enableVRS = false;
	bool m_VRSTier1Enabled = false;
#endif

	 
	enum Descriptors
	{
#ifdef FONTS_ENABLED
		MyFont,
#endif
#ifdef IMGUI_ENABLED
		Imgui,
#endif
		Count
	};
};
