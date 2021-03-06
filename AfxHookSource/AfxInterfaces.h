#pragma once

// Copyright (c) advancedfx.org
//
// Last changes:
// 2016-09-23 dominik.matrixstorm.com
//
// First changes:
// 2015-06-26 dominik.matrixstorm.com

#include "SourceInterfaces.h"
#include "csgo_Stdshader_dx9_Hooks.h"

#include <string>

class IAfxFreeable abstract
{
public:
	virtual void AfxFree(void) = 0;
};

class IAfxFreeMaster abstract
{
public:
	virtual void AfxFreeable_Register(IAfxFreeable * value) = 0;
	virtual void AfxFreeable_Unregister(IAfxFreeable * value) = 0;
};

class IAfxMaterial abstract
{
public:
	virtual SOURCESDK::IMaterial_csgo * GetMaterial() const = 0;
};

class IAfxVRenderView;

class IAfxVRenderViewSetBlend abstract
{
public:
	virtual void SetBlend(IAfxVRenderView * rv, float blend ) = 0;
};

class IAfxVRenderViewSetColorModulation abstract
{
public:
	virtual void SetColorModulation(IAfxVRenderView * rv, float const* blend ) = 0;
};

class IAfxVRenderView abstract
{
public:
	virtual SOURCESDK::IVRenderView_csgo * GetParent() = 0;
};


class IAfxMatRenderContext;
class IAfxStream;

class IAfxMatRenderContextMaterialHook abstract
{
public:
	/// <returns>Return the material you want to use (instead).</returns>
	virtual SOURCESDK::IMaterial_csgo * MaterialHook(IAfxMatRenderContext * ctx, SOURCESDK::IMaterial_csgo * material) = 0;
};

class IAfxMatRenderContextDrawInstances abstract
{
public:
	virtual void DrawInstances(IAfxMatRenderContext * ctx, int nInstanceCount, const SOURCESDK::MeshInstanceData_t_csgo *pInstance) = 0;
};

class IAfxContextHook;
class CAfxMatRenderContext;

class IAfxMatRenderContextOrg abstract
{
public:
	virtual void ClearBuffers(bool bClearColor, bool bClearDepth, bool bClearStencil = false) = 0; //:012

	virtual void ReadPixels(int x, int y, int width, int height, unsigned char *data, SOURCESDK::ImageFormat_csgo dstFormat, unsigned __int32 _unknown7 = 0) = 0; //:013

	virtual void ClearColor4ub(unsigned char r, unsigned char g, unsigned char b, unsigned char a) = 0; //:079

	virtual void PushRenderTargetAndViewport(SOURCESDK::ITexture_csgo *pTexture, SOURCESDK::ITexture_csgo *pDepthTexture, int nViewX, int nViewY, int nViewW, int nViewH) = 0; //:115

	virtual void PopRenderTargetAndViewport(void) = 0; //:119

	virtual SOURCESDK::CSGO::ICallQueue *GetCallQueue() = 0; //:150

	virtual void DrawInstances(int nInstanceCount, const SOURCESDK::MeshInstanceData_t_csgo *pInstance) = 0; //:192
};

class IAfxMatRenderContext abstract
{
public:
	virtual IAfxMatRenderContextOrg * GetOrg(void) = 0;

	virtual IAfxContextHook * Hook_get(void) = 0;

	virtual void Hook_set(IAfxContextHook * value) = 0;

	//virtual void * HookData_get(void) = 0;

	//virtual void * HookData_set(void * value) = 0;
};

class IAfxBaseClientDll;

class IAfxBaseClientDllShutdown abstract
{
public:
	virtual void Shutdown(IAfxBaseClientDll * cl) = 0;
};

class IAfxBaseClientDllLevelShutdown abstract
{
public:
	virtual void LevelShutdown(IAfxBaseClientDll * cl) = 0;
};

class IAfxBaseClientDllView_Render abstract
{
public:
	virtual void View_Render(IAfxBaseClientDll * cl, SOURCESDK::vrect_t_csgo *rect) = 0;
};

class IAfxBaseClientDll abstract
{
public:
	virtual SOURCESDK::IBaseClientDLL_csgo * GetParent() = 0;
	virtual IAfxFreeMaster * GetFreeMaster() = 0;

	virtual void OnShutdown_set(IAfxBaseClientDllShutdown * value) = 0;
	virtual void OnLevelShutdown_set(IAfxBaseClientDllLevelShutdown * value) = 0;
	virtual void OnView_Render_set(IAfxBaseClientDllView_Render * value) = 0;
};

class IAfxMesh abstract
{
public:
	virtual SOURCESDK::IMeshEx_csgo * GetParent(void) = 0;

	virtual IAfxMatRenderContext * GetContext(void) = 0;
};

class IAfxMeshDraw abstract
{
public:
	virtual void Draw(IAfxMesh * am, int firstIndex = -1, int numIndices = 0) = 0;
};

class IAfxMeshDraw_2 abstract
{
public:
	virtual void Draw_2(IAfxMesh * am, SOURCESDK::CPrimList_csgo *pLists, int nLists) = 0;
};

class IAfxMeshDrawModulated abstract
{
public:
	virtual void DrawModulated(IAfxMesh * am, const SOURCESDK::Vector4D_csgo &vecDiffuseModulation, int firstIndex = -1, int numIndices = 0 ) = 0;
};

class IAfxSetVertexShader abstract
{
public:
	virtual void SetVertexShader(CAfx_csgo_ShaderState & state) = 0;
};

class IAfxSetPixelShader abstract
{
public:
	virtual void SetPixelShader(CAfx_csgo_ShaderState & state) = 0;
};

class IAfxDrawingHud abstract
{
public:
	virtual void DrawingHud(void) = 0;
};

class IAfxCallQueue
{
public:
	virtual SOURCESDK::CSGO::ICallQueue * GetParent(void) = 0;
};
