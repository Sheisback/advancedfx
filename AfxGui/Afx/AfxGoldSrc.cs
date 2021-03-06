﻿// Copyright (c) by advancedfx.org
//
// Last changes:
// 2014-02-12 by dominik.matrixstorm.com
//
// First changes:
// 2009-12-06 by dominik.matrixstorm.com

using System;
using System.Threading;
using System.Windows.Forms;

using AfxGui;

namespace Afx {

class AfxGoldSrc : IDisposable
{
    //
    // Public members:

    public enum RenderMode
    {
        Default = 0,
        FrameBufferObject,
        MemoryDC
    }

    public class StartSettings
    {
        public bool Alpha8;
        public byte Bpp;
        public String CustomLaunchOptions;
        public bool ForceRes;
        public bool FullScreen;
        public String HalfLifePath;
        public uint Height;
        public String Modification;
        public bool OptWindowVisOnRec;
        public RenderMode RenderMode;
        public uint Width;

        public StartSettings(
            String halfLifePath,
            String modification
        )
        {
            Alpha8 = false;
            Bpp = 32;
            CustomLaunchOptions = "";
            ForceRes = true;
            FullScreen = false;
            HalfLifePath = halfLifePath;
            Height = 480;
            Modification = modification;
            OptWindowVisOnRec = true;
            RenderMode = RenderMode.Default;
            Width = 640;
        }
    }

    public AfxGoldSrc(MainForm mainForm)
    {
        m_MainForm = mainForm;

        m_PipeComServer = new PipeComServer();
    }

    public void Dispose()
    {
        if (m_Disposed) return;

        Stop();

        m_PipeComServer.Dispose();

        m_Disposed = true;
    }

    public bool Start(StartSettings startSettings)
    {
        Stop();

	    if(0 < System.Diagnostics.Process.GetProcessesByName( "hl" ).Length)
            return false;

        String cmds, s1;

	    //
	    //	build parameters:

	    cmds = "-steam -gl";
	
	    cmds += " -game " + startSettings.Modification;

	    // gfx settings

        cmds += " -nofbo";

        cmds += startSettings.FullScreen ? " -full -stretchaspect" : " -window";

	    s1 = startSettings.Bpp.ToString();
	    if( 0 < s1.Length) cmds += " -" + s1 + "bpp";

	    s1 = startSettings.Width.ToString();
	    if( 0 < s1.Length) cmds += " -w " + s1;

	    s1 = startSettings.Height.ToString();
	    if( 0 < s1.Length) cmds += " -h " + s1;

	    if(startSettings.ForceRes) cmds += " -forceres";

	    // pipe handles:
	    cmds += " -afxpipes "
            + m_PipeComServer.ClientReadPipe.ToString()
            + ","
		    + m_PipeComServer.ClientWritePipe.ToString()
	    ;

	    // advanced

	    // custom command line

	    s1 = startSettings.CustomLaunchOptions;
	    if( 0 < s1.Length)
		    cmds += " " + s1;

	    //
	    // Launch:

        m_StartSettings = startSettings;

        StartServer();

        if (!AfxCppCli.AfxHook.LauchAndHook(
            startSettings.HalfLifePath,
            cmds,
            System.Windows.Forms.Application.StartupPath + "\\AfxHookGoldSrc.dll"
        ))
        {
            //TODO:
            // this won't actually work properly yet, the sever cannot stop properly
            // once it's started atm.
            StopServer();
            return false;
        }


        return true;
    }

    public void Stop()
    {
        StopServer();

        m_StartSettings = null;
    }

    //
    // Public properties:

    //
    // Private members:

    enum ClientMessage
    {
	    EOT = 0,
	    Closed,
	    OnHostFrame,
	    OnRecordStarting,
	    OnRecordEnded,
	    UpdateWindowSize
    };

    enum ServerMessage
    {
	    EOT = 0,
	    Close
    };

    enum ComRenderMode
    {
	    Standard = 0,
	    FrameBufferObject,
	    MemoryDc
    };

    const int COM_VERSION = 0;

    bool m_Disposed;
    MainForm m_MainForm;
    PipeComServer m_PipeComServer;
    StartSettings m_StartSettings;
    bool m_ServerShutdown;
    Thread m_ServerThread;

    void ClientMessage_OnHostFrame()
    {
	    if(m_ServerShutdown)
		    SendMessage(ServerMessage.Close);
        SendMessage(ServerMessage.EOT);
    }

    void ClientMessage_UpdateWindowSize()
    {
	    int width, height;

        width = m_PipeComServer.ReadInt32();
        height = m_PipeComServer.ReadInt32();
    }

    IntPtr DoGetHandle(IWin32Window window)
    {
        return window.Handle;
    }


    ClientMessage RecvMessage()
    {
        return (ClientMessage)m_PipeComServer.ReadInt32();
    }

    void SendMessage(ServerMessage msg)
    {
        m_PipeComServer.Write((Int32)msg);
    }


    bool ServerInit()
    {
        m_PipeComServer.Write((Int32)COM_VERSION); // version

        if (!m_PipeComServer.ReadBoolean()) // ok?
            return false; // abort

        m_PipeComServer.Write((Int32)m_StartSettings.Width);
        m_PipeComServer.Write((Int32)m_StartSettings.Height);
        m_PipeComServer.Write((Boolean)m_StartSettings.Alpha8);
        m_PipeComServer.Write((Boolean)m_StartSettings.FullScreen);
        m_PipeComServer.Write((Boolean)m_StartSettings.OptWindowVisOnRec);
        m_PipeComServer.Write((Int32)0); // GameWindowParentHandle not supported atm
        m_PipeComServer.Write((Int32)m_StartSettings.RenderMode);

	    return true;
    }


    void ServerWorker()
    {
        if(!ServerInit())
            return;

	    ClientMessage clientMessage;

	    while(ClientMessage.Closed != (clientMessage = RecvMessage()))
	    {
		    switch(clientMessage)
		    {
            case ClientMessage.OnHostFrame: 
			    ClientMessage_OnHostFrame();
			    break;
            case ClientMessage.UpdateWindowSize:
                ClientMessage_UpdateWindowSize();
			    break;
		    }
	    }
    }

    void StartServer()
    {
        StopServer();

        m_ServerThread = new Thread(new ThreadStart(ServerWorker));
        m_ServerThread.Start();
    }

    void StopServer()
    {
        if (null != m_ServerThread && m_ServerThread.IsAlive)
        {
            m_ServerShutdown = true;
            m_ServerThread.Join();
            m_ServerShutdown = false;
        }
        m_ServerThread = null;
    }
}

} // namespace Afx {

