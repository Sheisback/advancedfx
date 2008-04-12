// Project :  Half-Life Advanced Effects
// File    :  dll/supportrender.h

// Authors : last change / first change / name
// 2008-04-12 / 2008-03-27 / Dominik Tugend
// 2007 / / Gavin Bramhill (parts from old ui.cpp for fbo swapbuffers)

// Comment: see supportrender.h

#include <windows.h>

#include <gl\gl.h>
#include <gl\glu.h>
#include "../shared\ogl\glext.h"

#include "wrect.h"
#include "cl_dll.h"
#include "cdll_int.h"
//#include "r_efx.h"
//#include "com_model.h"
//#include "r_studioint.h"
//#include "pm_defs.h"
//#include "cvardef.h"
//#include "entity_types.h"

#include "supportrender.h"

extern cl_enginefuncs_s *pEngfuncs;

//#define ERROR_MESSAGE(errmsg) pEngfuncs->Con_Printf("SupportRender: " errmsg "\n");

char g_ErrorMessageBuffer[300];

#define ERROR_MESSAGE(errmsg) MessageBoxA(0, errmsg, "SupportRender:",MB_OK|MB_ICONERROR);
#define ERROR_MESSAGE_LE(errmsg) \
	{ \
		_snprintf(g_ErrorMessageBuffer,sizeof(g_ErrorMessageBuffer)-1,"SupportRender:" errmsg "\nGetLastError: %i",::GetLastError()); \
		g_ErrorMessageBuffer[sizeof(g_ErrorMessageBuffer)-1]=0; \
		MessageBoxA(0, g_ErrorMessageBuffer, "SupportRender:",MB_OK|MB_ICONERROR); \
	}


//
// EXT_framebuffer_object
//
//
// http://www.opengl.org/registry/specs/EXT/framebuffer_object.txt
//

bool g_bEXT_framebuffer_object=false;

PFNGLISRENDERBUFFEREXTPROC glIsRenderbufferEXT = NULL;
PFNGLBINDRENDERBUFFEREXTPROC glBindRenderbufferEXT = NULL;
PFNGLDELETERENDERBUFFERSEXTPROC glDeleteRenderbuffersEXT = NULL;
PFNGLGENRENDERBUFFERSEXTPROC glGenRenderbuffersEXT = NULL;
PFNGLRENDERBUFFERSTORAGEEXTPROC glRenderbufferStorageEXT = NULL;
PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC glGetRenderbufferParameterivEXT = NULL;
PFNGLISFRAMEBUFFEREXTPROC glIsFramebufferEXT = NULL;
PFNGLBINDFRAMEBUFFEREXTPROC glBindFramebufferEXT = NULL;
PFNGLDELETEFRAMEBUFFERSEXTPROC glDeleteFramebuffersEXT = NULL;
PFNGLGENFRAMEBUFFERSEXTPROC glGenFramebuffersEXT = NULL;
PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC glCheckFramebufferStatusEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE1DEXTPROC glFramebufferTexture1DEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE2DEXTPROC glFramebufferTexture2DEXT = NULL;
PFNGLFRAMEBUFFERTEXTURE3DEXTPROC glFramebufferTexture3DEXT = NULL;
PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC glFramebufferRenderbufferEXT = NULL;
PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC glGetFramebufferAttachmentParameterivEXT = NULL;
PFNGLGENERATEMIPMAPEXTPROC glGenerateMipmapEXT = NULL;

CHlaeSupportRender::EFboSupport InstallFrameBufferExtentsion(void)
// ATTENTION:
//   do not install before a context has been created, because
//   in this case glGetString will not work (return 0)
{
	if (g_bEXT_framebuffer_object) return CHlaeSupportRender::FBOS_YES;

	CHlaeSupportRender::EFboSupport retTemp=CHlaeSupportRender::FBOS_NO;
	char *pExtStr = (char *)(glGetString( GL_EXTENSIONS ));
	if (!pExtStr)
	{
		retTemp = CHlaeSupportRender::FBOS_UNKNOWN;
		ERROR_MESSAGE("glGetString failed!\nInstallFrameBufferExtentsion called before context was currrent?")
	}
	else if (strstr( pExtStr, "EXT_framebuffer_object" ))
	{
		glIsRenderbufferEXT = (PFNGLISRENDERBUFFEREXTPROC)wglGetProcAddress("glIsRenderbufferEXT");
		glBindRenderbufferEXT = (PFNGLBINDRENDERBUFFEREXTPROC)wglGetProcAddress("glBindRenderbufferEXT");
		glDeleteRenderbuffersEXT = (PFNGLDELETERENDERBUFFERSEXTPROC)wglGetProcAddress("glDeleteRenderbuffersEXT");
		glGenRenderbuffersEXT = (PFNGLGENRENDERBUFFERSEXTPROC)wglGetProcAddress("glGenRenderbuffersEXT");
		glRenderbufferStorageEXT = (PFNGLRENDERBUFFERSTORAGEEXTPROC)wglGetProcAddress("glRenderbufferStorageEXT");
		glGetRenderbufferParameterivEXT = (PFNGLGETRENDERBUFFERPARAMETERIVEXTPROC)wglGetProcAddress("glGetRenderbufferParameterivEXT");
		glIsFramebufferEXT = (PFNGLISFRAMEBUFFEREXTPROC)wglGetProcAddress("glIsFramebufferEXT");
		glBindFramebufferEXT = (PFNGLBINDFRAMEBUFFEREXTPROC)wglGetProcAddress("glBindFramebufferEXT");
		glDeleteFramebuffersEXT = (PFNGLDELETEFRAMEBUFFERSEXTPROC)wglGetProcAddress("glDeleteFramebuffersEXT");
		glGenFramebuffersEXT = (PFNGLGENFRAMEBUFFERSEXTPROC)wglGetProcAddress("glGenFramebuffersEXT");
		glCheckFramebufferStatusEXT = (PFNGLCHECKFRAMEBUFFERSTATUSEXTPROC)wglGetProcAddress("glCheckFramebufferStatusEXT");
		glFramebufferTexture1DEXT = (PFNGLFRAMEBUFFERTEXTURE1DEXTPROC)wglGetProcAddress("glFramebufferTexture1DEXT");
		glFramebufferTexture2DEXT = (PFNGLFRAMEBUFFERTEXTURE2DEXTPROC)wglGetProcAddress("glFramebufferTexture2DEXT");
		glFramebufferTexture3DEXT = (PFNGLFRAMEBUFFERTEXTURE3DEXTPROC)wglGetProcAddress("glFramebufferTexture3DEXT");
		glFramebufferRenderbufferEXT = (PFNGLFRAMEBUFFERRENDERBUFFEREXTPROC)wglGetProcAddress("glFramebufferRenderbufferEXT");
		glGetFramebufferAttachmentParameterivEXT = (PFNGLGETFRAMEBUFFERATTACHMENTPARAMETERIVEXTPROC)wglGetProcAddress("glGetFramebufferAttachmentParameterivEXT");
		glGenerateMipmapEXT = (PFNGLGENERATEMIPMAPEXTPROC)wglGetProcAddress("glGenerateMipmapEXT");

		if (
			glIsRenderbufferEXT && glBindRenderbufferEXT && glDeleteRenderbuffersEXT && glGenRenderbuffersEXT
			&& glRenderbufferStorageEXT && glGetRenderbufferParameterivEXT && glIsFramebufferEXT && glBindFramebufferEXT
			&& glDeleteFramebuffersEXT && glGenFramebuffersEXT && glCheckFramebufferStatusEXT && glFramebufferTexture1DEXT
			&& glFramebufferTexture2DEXT && glFramebufferTexture3DEXT && glFramebufferRenderbufferEXT && glGetFramebufferAttachmentParameterivEXT
			&& glGenerateMipmapEXT
		) retTemp = CHlaeSupportRender::FBOS_YES;


	}

	g_bEXT_framebuffer_object = CHlaeSupportRender::FBOS_YES == retTemp;

	return retTemp;
}

//
//  CHlaeSupportRender
//

CHlaeSupportRender::CHlaeSupportRender(HWND hGameWindow, int iWidth, int iHeight)
{
	_hGameWindow = hGameWindow;
	_iWidth = iWidth;
	_iHeight = iHeight;

	_eRenderTarget = RT_NULL;
	_eFBOsupported = FBOS_UNKNOWN;

	_ownHGLRC = NULL;
}

CHlaeSupportRender::~CHlaeSupportRender()
{
	if (RT_NULL != _eRenderTarget) hlaeDeleteContext (_ownHGLRC);
	_eRenderTarget = RT_NULL;
}

CHlaeSupportRender::EFboSupport CHlaeSupportRender::Has_EXT_FrameBufferObject()
{
	return _eFBOsupported;
}

CHlaeSupportRender::ERenderTarget CHlaeSupportRender::GetRenderTarget()
{
	return _eRenderTarget;
}

HGLRC CHlaeSupportRender::GetHGLRC()
{
	return _ownHGLRC;
}

HDC	CHlaeSupportRender::GetInternalHDC()
{
	switch (_eRenderTarget)
	{
	case RT_MEMORYDC:
		return _MemoryDc_r.ownHDC;
	case RT_HIDDENWINDOW:
		return _HiddenWindow_r.ownHDC;
	}
	return NULL;
}

HWND CHlaeSupportRender::GetInternalHWND()
{
	if (_eRenderTarget == RT_HIDDENWINDOW)
		return _HiddenWindow_r.ownHWND;

	return NULL;
}

HGLRC CHlaeSupportRender::hlaeCreateContext (ERenderTarget eRenderTarget, HDC hGameWindowDC)
{
	if(RT_NULL != _eRenderTarget)
	{
		ERROR_MESSAGE("already using a target")
		return NULL;
	}

	switch (eRenderTarget)
	{
	case RT_NULL:
		ERROR_MESSAGE("cannot Create RT_NULL target")
		return NULL;
	case RT_GAMEWINDOW:
		return _Create_RT_GAMEWINDOW (hGameWindowDC);
	case RT_MEMORYDC:
		return _Create_RT_MEMORYDC (hGameWindowDC);
	case RT_FRAMEBUFFEROBJECT:
		return _Create_RT_FRAMEBUFFEROBJECT (hGameWindowDC);
	}

	ERROR_MESSAGE("cannot Create unknown target")
	return NULL;
}

BOOL CHlaeSupportRender::hlaeDeleteContext (HGLRC hGlRc)
{
	if (RT_NULL==_eRenderTarget)
	{
		ERROR_MESSAGE("cannot Delete RT_NULL target")
		return FALSE;
	}
	else if (hGlRc != _ownHGLRC)
	{
		ERROR_MESSAGE("cannot Delete, hGlRc is not managed by this class")
		return FALSE;
	}

	switch (_eRenderTarget)
	{
	case RT_GAMEWINDOW:
		return _Delete_RT_GAMEWINDOW ();
	case RT_MEMORYDC:
		return _Delete_RT_MEMORYDC ();
	case RT_FRAMEBUFFEROBJECT:
		return _Delete_RT_FRAMEBUFFEROBJECT ();
	}

	ERROR_MESSAGE("cannot delete unknown target")
	return FALSE;
}

BOOL CHlaeSupportRender::hlaeMakeCurrent(HDC hGameWindowDC, HGLRC hGlRc)
{
	if (RT_NULL==_eRenderTarget)
	{
		ERROR_MESSAGE("cannot MakeCurrent RT_NULL target")
		return FALSE;
	}
	else if (hGlRc != _ownHGLRC)
	{
		ERROR_MESSAGE("cannot MakeCurrent, hGlRc is not managed by this class")
		return FALSE;
	}

	switch (_eRenderTarget)
	{
	case RT_GAMEWINDOW:
		return wglMakeCurrent(hGameWindowDC,_ownHGLRC);
	case RT_MEMORYDC:
		return _MakeCurrent_RT_MEMORYDC (hGameWindowDC);
	case RT_FRAMEBUFFEROBJECT:
		return _MakeCurrent_RT_FRAMEBUFFEROBJECT (hGameWindowDC);
	}

	ERROR_MESSAGE("cannot MakeCurrent unknown target")
	return FALSE;
}

BOOL CHlaeSupportRender::hlaeSwapBuffers(HDC hGameWindowDC)
{
	if (RT_NULL==_eRenderTarget)
	{
		ERROR_MESSAGE("cannot SwapBuffers RT_NULL target")
		return FALSE;
	}

	switch (_eRenderTarget)
	{
	case RT_GAMEWINDOW:
		return SwapBuffers (hGameWindowDC);
	case RT_MEMORYDC:
		return _SwapBuffers_RT_MEMORYDC (hGameWindowDC);
	case RT_FRAMEBUFFEROBJECT:
		return _SwapBuffers_RT_FRAMEBUFFEROBJECT (hGameWindowDC);
	}

	ERROR_MESSAGE("cannot SwapBuffers unknown target")
	return FALSE;
}

HGLRC CHlaeSupportRender::_Create_RT_GAMEWINDOW (HDC hGameWindowDC)
{
	_ownHGLRC = wglCreateContext(hGameWindowDC);

	if(_ownHGLRC) _eRenderTarget=RT_GAMEWINDOW;

	return _ownHGLRC;
}

BOOL CHlaeSupportRender::_Delete_RT_GAMEWINDOW ()
{
	BOOL wbRet = wglDeleteContext(_ownHGLRC);

	if (TRUE != wbRet)
		return wbRet;

	_eRenderTarget=RT_NULL;
	_ownHGLRC=NULL;
	
	return wbRet;
}

HGLRC CHlaeSupportRender::_Create_RT_MEMORYDC (HDC hGameWindowDC)
{
	_MemoryDc_r.ownHDC = CreateCompatibleDC(hGameWindowDC);
	if (!_MemoryDc_r.ownHDC)
	{
		ERROR_MESSAGE_LE("could not create compatible context")
		return NULL;
	}

    _MemoryDc_r.ownHBITMAP = CreateCompatibleBitmap ( hGameWindowDC, _iWidth, _iHeight );
	if (!_MemoryDc_r.ownHBITMAP)
	{
		ERROR_MESSAGE_LE("could not create compatible bitmap")
		DeleteDC(_MemoryDc_r.ownHDC);
		return NULL;
	}

    HGDIOBJ tobj = SelectObject ( _MemoryDc_r.ownHDC, _MemoryDc_r.ownHBITMAP );
	if (!tobj || tobj == HGDI_ERROR)
	{
		ERROR_MESSAGE_LE("could not select bitmap")
		DeleteObject(_MemoryDc_r.ownHBITMAP);
		DeleteDC(_MemoryDc_r.ownHDC);
		return NULL;
	}

	int iPixelFormat = GetPixelFormat( hGameWindowDC );
	PIXELFORMATDESCRIPTOR *ppfd = new (PIXELFORMATDESCRIPTOR);
	DescribePixelFormat( hGameWindowDC, iPixelFormat, sizeof(PIXELFORMATDESCRIPTOR), ppfd );

	ppfd->dwFlags = (ppfd->dwFlags & !( (DWORD)PFD_DRAW_TO_WINDOW | (DWORD)PFD_DOUBLEBUFFER  )) | PFD_DRAW_TO_BITMAP;
	//ppfd->dwFlags = PFD_DRAW_TO_BITMAP | PFD_SUPPORT_OPENGL | PFD_GENERIC_ACCELERATED;

	iPixelFormat = ChoosePixelFormat(_MemoryDc_r.ownHDC,ppfd);
	if (iPixelFormat == 0)
	{
		ERROR_MESSAGE_LE("could not choose PixelFormat")
		delete ppfd;
		DeleteObject(_MemoryDc_r.ownHBITMAP);
		DeleteDC(_MemoryDc_r.ownHDC);
		return NULL;
	}

	if (TRUE != SetPixelFormat(_MemoryDc_r.ownHDC,iPixelFormat,ppfd))
	{
		ERROR_MESSAGE_LE("could not Set PixelFormat")
		delete ppfd;
		DeleteObject(_MemoryDc_r.ownHBITMAP);
		DeleteDC(_MemoryDc_r.ownHDC);
		return NULL;
	}

	delete ppfd;

	_ownHGLRC = wglCreateContext(_MemoryDc_r.ownHDC);
	if (!_ownHGLRC)
	{
		ERROR_MESSAGE_LE("could not create own context")
		DeleteObject(_MemoryDc_r.ownHBITMAP);
		DeleteDC(_MemoryDc_r.ownHDC);
		return NULL;
	}

	_eRenderTarget=RT_MEMORYDC;

	return _ownHGLRC;
}

BOOL CHlaeSupportRender::_Delete_RT_MEMORYDC ()
{
	BOOL wbRet = wglDeleteContext(_ownHGLRC);
	
	if (TRUE != wbRet)
		return wbRet;

	_eRenderTarget=RT_NULL;
	_ownHGLRC=NULL;

	DeleteObject(_MemoryDc_r.ownHBITMAP);
	DeleteDC(_MemoryDc_r.ownHDC);

	return wbRet;
}

BOOL CHlaeSupportRender::_MakeCurrent_RT_MEMORYDC (HDC hGameWindowDC)
{
	return wglMakeCurrent( _MemoryDc_r.ownHDC, _ownHGLRC );
}

BOOL CHlaeSupportRender::_SwapBuffers_RT_MEMORYDC (HDC hGameWindowDC)
{
	BOOL bwRet=FALSE;
	bwRet=BitBlt(hGameWindowDC,0,0,_iWidth,_iHeight,_MemoryDc_r.ownHDC,0,0,SRCCOPY);
	//SwapBuffers(hGameWindowDC);
	return bwRet;
}

HGLRC CHlaeSupportRender::_Create_RT_FRAMEBUFFEROBJECT (HDC hGameWindowDC)
{
	_ownHGLRC = wglCreateContext(hGameWindowDC);
	
	if(_ownHGLRC) _eRenderTarget=RT_FRAMEBUFFEROBJECT;

	return _ownHGLRC;
}

BOOL CHlaeSupportRender::_Delete_RT_FRAMEBUFFEROBJECT ()
{
	_Delete_RT_FRAMEBUFFEROBJECT_onlyFBO ();

	BOOL wbRet = wglDeleteContext(_ownHGLRC);

	if (TRUE != wbRet)
		return wbRet;

	_eRenderTarget=RT_NULL;
	_ownHGLRC=NULL;
	
	return wbRet;
}

BOOL CHlaeSupportRender::_MakeCurrent_RT_FRAMEBUFFEROBJECT (HDC hGameWindowDC)
{
	BOOL bwResult = wglMakeCurrent( hGameWindowDC, _ownHGLRC );

	// since we have a current context we can also query the extensions now:
	// (WHY DOES THIS NOT WORK EARLIER BTW?, CAN WE DETECT THAT EARLIER?)
	_eFBOsupported = InstallFrameBufferExtentsion();

	if (FBOS_YES != _eFBOsupported)
	{
		if (FBOS_NO == _eFBOsupported)
			ERROR_MESSAGE("EXT_FrameBufferObject not supported!\nFalling back to RT_GAMEWINDOW ...")
		else
			ERROR_MESSAGE("EXT_FrameBufferObject support could not be evaluated!\nFalling back to RT_GAMEWINDOW ...")
		
		_eRenderTarget = RT_GAMEWINDOW;

		return bwResult;
	}

	// create rgbaRenderTexture:
	//glGenTextures( 1, &_FrameBufferObject_r.rgbaRenderTexture );
	_FrameBufferObject_r.rgbaRenderTexture =  FBO_TEXUTRE_ID;
	glBindTexture( GL_TEXTURE_2D, _FrameBufferObject_r.rgbaRenderTexture );
	glGetError();
	glTexImage2D( GL_TEXTURE_2D, 0, GL_RGBA, _iWidth, _iHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, 0 );
	if (glGetError()!=0)
	{
		ERROR_MESSAGE("Failed to create texture with required properties!\nFalling back to RT_GAMEWINDOW ...")
		glDeleteTextures( 1, &_FrameBufferObject_r.rgbaRenderTexture );

		_eRenderTarget = RT_GAMEWINDOW;

		return bwResult;
	}
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);

	// create FrameBufferObject:
	glGenFramebuffersEXT( 1, &_FrameBufferObject_r.FBOid );

	// create depthRenderBuffer:
	glGenRenderbuffersEXT( 1, &_FrameBufferObject_r.depthRenderBuffer );
	glBindRenderbufferEXT( GL_RENDERBUFFER_EXT, _FrameBufferObject_r.depthRenderBuffer );
	glRenderbufferStorageEXT( GL_RENDERBUFFER_EXT, GL_DEPTH_COMPONENT, _iWidth, _iHeight );

	// bind and setup FrameBufferObject (select buffers / textures):
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, _FrameBufferObject_r.FBOid );
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _FrameBufferObject_r.rgbaRenderTexture, 0 );
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, _FrameBufferObject_r.depthRenderBuffer );

	// check if FBO status is complete:
	if (GL_FRAMEBUFFER_COMPLETE_EXT != glCheckFramebufferStatusEXT( GL_FRAMEBUFFER_EXT ))
	{
		ERROR_MESSAGE("FrameBufferObject status not complete\nFalling back to RT_GAMEWINDOW ...")

		_Delete_RT_FRAMEBUFFEROBJECT_onlyFBO ();

		_eRenderTarget=RT_GAMEWINDOW;

		return bwResult;
	}
		

	return bwResult;
}

BOOL CHlaeSupportRender::_SwapBuffers_RT_FRAMEBUFFEROBJECT (HDC hGameWindowDC)
// thx to mst havoc for extensive testing!
{
	BOOL bwRet=FALSE;
	struct
	{
		GLint matrixmode;
		GLint polygonmode[2];
		GLfloat colours[4];
		GLint texture2d;
		GLboolean alpha;
		GLboolean lighting;
		GLboolean texture;
		GLboolean blend;
		GLboolean cull;
		GLboolean depth;
	} stateBackup;

	//
	// switch to window-system frambuffer obj:
	//
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

	//
	// Backup old state of things we are going to change:
	//

	glGetIntegerv(GL_MATRIX_MODE, &stateBackup.matrixmode);
	glGetIntegerv(GL_POLYGON_MODE, stateBackup.polygonmode);
	glGetFloatv(GL_CURRENT_COLOR, stateBackup.colours);
	glGetIntegerv(GL_TEXTURE_BINDING_2D, &stateBackup.texture2d);

	stateBackup.alpha = glIsEnabled(GL_ALPHA_TEST);
	stateBackup.lighting = glIsEnabled(GL_LIGHTING);
	stateBackup.texture = glIsEnabled(GL_TEXTURE_2D);
	stateBackup.blend = glIsEnabled(GL_BLEND);
	stateBackup.cull = glIsEnabled(GL_CULL_FACE);
	stateBackup.depth = glIsEnabled(GL_DEPTH_TEST);

	//
	// Set new states and draw:
	//

	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//glDrawBuffer(GL_BACK);

	// Change projection matrix to orth
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, _iWidth, _iHeight, 0);

	// Back to model for rendering
	glMatrixMode(GL_MODELVIEW);
	//glPushMatrix();
	glLoadIdentity();

	glDisable(GL_ALPHA_TEST);
	glDisable(GL_LIGHTING);
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);

	glColor4f(1.0f,1.0f,1.0f,1.0f);
	
	glBindTexture(GL_TEXTURE_2D, _FrameBufferObject_r.rgbaRenderTexture ); // bind fbo texture
	//glBindTexture(GL_TEXTURE_2D, 0 );

	// Draw fbo texture quad on screen:
	glBegin(GL_QUADS);
		glTexCoord2f(0.0f, 1.0f); glVertex3f(    0.0f,     0.0f, 0.0f);	// Top Left
		glTexCoord2f(1.0f, 1.0f); glVertex3f( _iWidth,     0.0f, 0.0f);	// Top Right
		glTexCoord2f(1.0f, 0.0f); glVertex3f( _iWidth, _iHeight, 0.0f);	// Bottom Right
		glTexCoord2f(0.0f, 0.0f); glVertex3f(    0.0f, _iHeight, 0.0f);	// Bottom Left
	glEnd();
	

	//
	// swap window display:
	//

	bwRet = SwapBuffers(hGameWindowDC); 

	//
	// Restore old states:
	//

	glColor4fv(stateBackup.colours);
	glPolygonMode(stateBackup.polygonmode[0],stateBackup.polygonmode[1]);
	glBindTexture(GL_TEXTURE_2D,stateBackup.texture2d);

	#define GLSETENABLED(cap,mode) if (mode) glEnable(cap);
	#define GLSETDISABLED(cap,mode) if(!mode) glDisable(cap);
	GLSETENABLED(	GL_ALPHA_TEST, stateBackup.alpha )
	GLSETENABLED(	GL_LIGHTING, stateBackup.lighting )
	GLSETDISABLED(	GL_TEXTURE_2D, stateBackup.texture )
	GLSETENABLED(	GL_CULL_FACE, stateBackup.cull )
	GLSETENABLED(	GL_DEPTH_TEST, stateBackup.depth )
	GLSETENABLED(	GL_BLEND, stateBackup.blend )

	//glPopMatrix();

	// Reset the projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();

	// Back to whatever we were before
	glMatrixMode(stateBackup.matrixmode);

	//
	// bind our FBO back for rendering:
	//
	
	glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, _FrameBufferObject_r.FBOid );
	glFramebufferTexture2DEXT( GL_FRAMEBUFFER_EXT, GL_COLOR_ATTACHMENT0_EXT, GL_TEXTURE_2D, _FrameBufferObject_r.rgbaRenderTexture, 0 );
	glFramebufferRenderbufferEXT( GL_FRAMEBUFFER_EXT, GL_DEPTH_ATTACHMENT_EXT, GL_RENDERBUFFER_EXT, _FrameBufferObject_r.depthRenderBuffer );

	return bwRet;
}

void CHlaeSupportRender::_Delete_RT_FRAMEBUFFEROBJECT_onlyFBO ()
{
	if (_FrameBufferObject_r.FBOid)
	{
		glBindFramebufferEXT( GL_FRAMEBUFFER_EXT, 0 );

		glDeleteFramebuffersEXT( 1, &_FrameBufferObject_r.FBOid );
		_FrameBufferObject_r.FBOid = 0;

		glDeleteTextures( 1, &_FrameBufferObject_r.rgbaRenderTexture );
		glDeleteRenderbuffersEXT( 1, &_FrameBufferObject_r.depthRenderBuffer );
	}
}
