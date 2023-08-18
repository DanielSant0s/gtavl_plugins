// tga2fntDlg.cpp : implementation file
//

#include "stdafx.h"
#include "tga2fnt.h"
#include "tga2fntDlg.h"

#include "../common/tga.h"
#include "../common/iif.h"
#include "../common/bfnt.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CAboutDlg dialog used for App About

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// Dialog Data
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

// Implementation
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Ctga2fntDlg dialog



Ctga2fntDlg::Ctga2fntDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Ctga2fntDlg::IDD, pParent)
	, tga_filename(_T(""))
	, dat_filename(_T(""))
	, CharWidth(16)
	, CharHeight(16)
	, GridWidth(16)
	, GridHeight(16)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Ctga2fntDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_TGA_FILENAME, tga_filename);
	DDX_Text(pDX, IDC_DAT_FILENAME, dat_filename);
	DDX_Text(pDX, IDC_PIXWIDTH, CharWidth);
	DDV_MinMaxUInt(pDX, CharWidth, 1, 256);
	DDX_Text(pDX, IDC_PIXHEIGHT, CharHeight);
	DDV_MinMaxUInt(pDX, CharHeight, 1, 256);
	DDX_Text(pDX, IDC_GRIDWIDTH, GridWidth);
	DDV_MinMaxUInt(pDX, GridWidth, 1, 256);
	DDX_Text(pDX, IDC_GRIDHEIGHT, GridHeight);
	DDV_MinMaxUInt(pDX, GridHeight, 1, 256);
}

BEGIN_MESSAGE_MAP(Ctga2fntDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_TGA_PICK, OnBnClickedTgaPick)
	ON_BN_CLICKED(IDC_DAT_PICK, OnBnClickedDatPick)
	ON_BN_CLICKED(IDOK, OnBnClickedOk)
END_MESSAGE_MAP()


// Ctga2fntDlg message handlers

BOOL Ctga2fntDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// Add "About..." menu item to system menu.

	// IDM_ABOUTBOX must be in the system command range.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// Set the icon for this dialog.  The framework does this automatically
	//  when the application's main window is not a dialog
	SetIcon(m_hIcon, TRUE);			// Set big icon
	SetIcon(m_hIcon, FALSE);		// Set small icon

	// TODO: Add extra initialization here
	
	return TRUE;  // return TRUE  unless you set the focus to a control
}

void Ctga2fntDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void Ctga2fntDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // device context for painting

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Center icon in client rectangle
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// Draw the icon
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// The system calls this function to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR Ctga2fntDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void Ctga2fntDlg::OnBnClickedTgaPick()
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg(TRUE, ".tga", NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, "TGA Bitmap File (*.tga)|*.tga||", this);

	FileDlg.DoModal();

	tga_filename = FileDlg.GetPathName();

	// Update the dialog box with the filename
	UpdateData(FALSE);
}

void Ctga2fntDlg::OnBnClickedDatPick()
{
	// TODO: Add your control notification handler code here
	CFileDialog FileDlg(TRUE, ".tga", NULL, OFN_FILEMUSTEXIST | OFN_HIDEREADONLY, "Font Metric Data File (*.dat)|*.dat||", this);

	FileDlg.DoModal();

	dat_filename = FileDlg.GetPathName();

	// Update the dialog box with the filename
	UpdateData(FALSE);
}

void Ctga2fntDlg::OnBnClickedOk()
{
	TGA_HEADER	tgaHeader;
	FNT_HEADER	fntHeader;

	// Get all the data from the dialog
	UpdateData(TRUE);


	CFileDialog FileDlg(FALSE, ".fnt", NULL, OFN_HIDEREADONLY, "Bitmap Font File (*.fnt)|*.fnt||", this);

	FileDlg.DoModal();

	CString fnt_filename = FileDlg.GetPathName();


	// Read the TGA file header
	CFile tgaFile((LPCTSTR)tga_filename, CFile::modeRead);
	tgaFile.Read(&tgaHeader, 18);

	if ((tgaHeader.imagetype & 0x07) != 2)
	{
		MessageBox("Non-RGB images not yet supported","Error Message",MB_OK);
		OnOK();
		return;
	}

	if (tgaHeader.imagetype >= 8)
	{
		MessageBox("RLE packed images not yet supported","Error Message",MB_OK);
		OnOK();
		return;
	}

	if (tgaHeader.bits != 32)
	{
		MessageBox("Only 32bit bitmaps currently supported","Error Message",MB_OK);
		OnOK();
		return;
	}



	// Create the header of the .fnt file

	fntHeader.ID[0] = 'B';
	fntHeader.ID[1] = 'F';
	fntHeader.ID[2] = 'N';
	fntHeader.ID[3] = 'T';

	fntHeader.TexWidth = tgaHeader.width;
	fntHeader.TexHeight = tgaHeader.height;

	// Size of one char in grid
	fntHeader.CharGridWidth = CharWidth;
	fntHeader.CharGridHeight = CharHeight;

	// number of characters in grid
	fntHeader.NumXChars = GridWidth;
	fntHeader.NumYChars = GridHeight;

	switch(tgaHeader.bits)
	{
	case 8:
		fntHeader.PSM = 0x13;
		break;
	case 16:
		fntHeader.PSM = 0x02;
		break;
	case 24:
		fntHeader.PSM = 0x12;
		break;
	case 32:
	default:
		fntHeader.PSM = 0x00;
		break;
	}

	CFile fntFile((LPCTSTR)fnt_filename, CFile::modeCreate | CFile::modeWrite);
	fntFile.Write(&fntHeader, sizeof(fntHeader));

	// Copy the font-metrics data into the .fnt file
	CFile datFile((LPCTSTR)dat_filename, CFile::modeRead);

	unsigned char* datBuffer = (unsigned char*)malloc(256);
	datFile.Read(datBuffer,256);

	fntFile.Write(datBuffer, 256);
	free(datBuffer);

	// Copy the bitmap data into the .fnt data
	// (Correcting the alpha value if a 32bit image)

	int numpixels = tgaHeader.width * tgaHeader.height;

	TGA32_PIXEL* tgaBuffer = (TGA32_PIXEL*)malloc(numpixels*sizeof(TGA32_PIXEL));
	RGBA32_PIXEL* fntBuffer = (RGBA32_PIXEL*)malloc(numpixels*sizeof(RGBA32_PIXEL));

	tgaFile.Read(tgaBuffer,numpixels*sizeof(TGA32_PIXEL));
    
	// re-order the colour data, and halve the alpha values of each pixel
	for (int pix=0; pix<numpixels; pix++) // Every 4th byte
	{
		fntBuffer[pix].Red   = tgaBuffer[pix].Red;
		fntBuffer[pix].Green = tgaBuffer[pix].Green;
		fntBuffer[pix].Blue  = tgaBuffer[pix].Blue;
		fntBuffer[pix].Alpha = tgaBuffer[pix].Alpha/2;
	}

	fntFile.Write(fntBuffer, numpixels*sizeof(RGBA32_PIXEL));

	// Close all the files
	datFile.Close();
	tgaFile.Close();
	fntFile.Close();

	OnOK();
}
