
EE_OBJS_DIR = obj/


ITO_OBJS = PureVirtual.o 
ITO_OBJS := $(ITO_OBJS:%=$(EE_OBJS_DIR)Ito_%)

ITO_ASM_OBJS = ScratchpadStack.o
ITO_ASM_OBJS := $(ITO_ASM_OBJS:%=$(EE_OBJS_DIR)Ito_%)

OPERATORS_OBJS = Operators_new.o Operators_new2.o Operators_delete.o Operators_delete2.o
OPERATORS_OBJS := $(OPERATORS_OBJS:%=$(EE_OBJS_DIR)%)

FPUMATH_OBJS = Sqrt.o Cos.o Sin.o V_Unit.o V_Add.o V_Sub.o V_Mult.o V_DivXYZ.o V_Div.o V_ScaleXYZ.o V_Scale.o V_Clamp.o V_Normalize.o V_DotProduct.o
FPUMATH_OBJS += M_Equal.o M_Equal2.o M_Unit.o M_Mult.o M_Print.o M_RotateX.o M_RotateY.o M_RotateZ.o M_Rotate.o M_Translate.o M_Translate2.o M_Transpose.o
FPUMATH_OBJS += M_Perspective.o M_Frustum.o M_LookAt.o M_Viewport.o M_Viewscreen.o V_Constructor.o V_ScreenTransform.o V_ApplyMatrix.o
FPUMATH_OBJS += V_Operator1.o M_Operator.o V_Constructor2.o CalculateLight.o V_ApplyMatrix3x3.o M_Rotate2.o
FPUMATH_OBJS := $(FPUMATH_OBJS:%=$(EE_OBJS_DIR)FpuMath_%)

VU0MATH_OBJS = Sqrt.o Cos.o Sin.o V_Unit.o V_Add.o V_Sub.o V_Mult.o V_DivXYZ.o V_Div.o V_ScaleXYZ.o V_Scale.o V_Clamp.o V_Normalize.o V_DotProduct.o
VU0MATH_OBJS += M_Equal.o M_Equal2.o M_Unit.o M_Mult.o M_Print.o M_RotateX.o M_RotateY.o M_RotateZ.o M_Rotate.o M_Translate.o M_Translate2.o M_Transpose.o
VU0MATH_OBJS += M_Perspective.o M_Frustum.o M_LookAt.o M_Viewport.o M_Viewscreen.o V_Constructor.o V_ScreenTransform.o V_ApplyMatrix.o
VU0MATH_OBJS += V_Operator1.o M_Operator.o V_Constructor2.o CalculateLight.o V_ApplyMatrix3x3.o
VU0MATH_OBJS := $(VU0MATH_OBJS:%=$(EE_OBJS_DIR)Vu0Math_%)

VU0MATHASM_OBJS = MacroMatrixMult.o MacroSinCos.o VpuReset.o
VU0MATHASM_OBJS := $(VU0MATHASM_OBJS:%=$(EE_OBJS_DIR)Vu0MathAsm_%)


GIFPACKET_OBJS  = Constructor.o Constructor2.o Destructor.o Send.o Dump.o AddData.o GetUsedSize.o GetTotalSize.o Reset.o Wait.o AddAD.o End.o
GIFPACKET_OBJS += Giftag.o SetContext.o GetContext.o AddDWord.o AddQWord.o AddQWord2.o AddUploadTexture.o AddSetRegister.o AddSetTexture.o
GIFPACKET_OBJS += DmaTagRef.o DmaTagCnt.o DmaTagEnd.o AllocateDmaTag.o Check.o UpdateLastDmaTag.o
GIFPACKET_OBJS := $(GIFPACKET_OBJS:%=$(EE_OBJS_DIR)GifPacket_%)

TEXTUREMANAGER_OBJS = Constructor.o Destructor.o GetInstance.o TextureConstructor1.o TextureConstructor2.o TextureConstructor3.o TextureDestructor.o TextureSize.o
TEXTUREMANAGER_OBJS += Allocate.o FreeAll.o Free.o TextureBase.o TextureDump.o TextureIsAllocated.o Tick.o CreateTexture.o CreateClut.o TextureLoadImageBuffer.o
TEXTUREMANAGER_OBJS += TextureLoadImageFile.o TextureAllocate.o TextureFree.o TextureFreeFile.o TextureIsInVram.o TextureIsClut.o TexturePsm.o TextureCsm.o
TEXTUREMANAGER_OBJS += TextureKeep.o TextureHasClut.o CreateTexture2.o TextureAlwaysUpload.o
TEXTUREMANAGER_OBJS := $(TEXTUREMANAGER_OBJS:%=$(EE_OBJS_DIR)TextureManager_%)

VRAMMANAGER_OBJS = alloc.o allocTexture.o allocClut.o free.o freeTexture.o freeClut.o alloc2.o free2.o global.o
VRAMMANAGER_OBJS := $(VRAMMANAGER_OBJS:%=$(EE_OBJS_DIR)VramManager_%)

GSDRIVER_OBJS  = Constructor.o Destructor.o SetupEnv.o ShowNextFramebuffer.o SetNextDrawBuffer.o LoadImageBuffer.o Upload.o SetRegister.o
GSDRIVER_OBJS := $(GSDRIVER_OBJS:%=$(EE_OBJS_DIR)GsDriver_%)

GIFPIPE_OBJS  = Constructor.o Constructor2.o Destructor.o Line4.o Triangle4.o Sprite4.o ClearScreen.o Send.o Finish.o CheckPacket.o
GIFPIPE_OBJS += GifVRHandler.o Point4.o AlphaBlending.o AlphaTest.o ZTest.o SetRegister.o SetTexture.o LoadImageBuffer.o LoadImageFile.o
GIFPIPE_OBJS += UploadTexture.o TextureSprite.o TextureSprite2.o PrimAntialias.o PrimShading.o SetNextDrawBuffer.o PrimAlphaBlend.o 
GIFPIPE_OBJS += AlphaBlending2.o
GIFPIPE_OBJS := $(GIFPIPE_OBJS:%=$(EE_OBJS_DIR)GifPipe_%)

GS_OBJS =  SetBGCOLOR.o SetPMODE.o SetSMODE2.o SetDISPFB1.o SetDISPFB2.o SetDISPLAY1.o SetDISPLAY2.o TextureBufferBase.o
GS_OBJS += Reset.o VSyncWait.o PrintControlRegisters.o GetVideoMode.o GetBGCOLOR.o GetPixelSize.o GetTextureSize.o GetZPixelSize.o
GS_OBJS := $(GS_OBJS:%=$(EE_OBJS_DIR)Gs_%)

PAD_OBJS =  Constructor.o Destructor.o Open.o Close.o GetState.o ReadButtons.o
PAD_OBJS := $(PAD_OBJS:%=$(EE_OBJS_DIR)Pad_%)

THREAD_OBJS =  Constructor.o Destructor.o Start.o Suspend.o iSuspend.o Resume.o iResume.o Wakeup.o iWakeup.o Delete.o ChangePriority.o
THREAD_OBJS += iChangePriority.o Terminate.o iTerminate.o Exit.o ExitDelete.o Create.o
THREAD_OBJS := $(THREAD_OBJS:%=$(EE_OBJS_DIR)Thread_%)

SEMA_OBJS =  Constructor.o Destructor.o Create.o Delete.o iDelete.o Signal.o iSignal.o Poll.o iPoll.o Wait.o
SEMA_OBJS := $(SEMA_OBJS:%=$(EE_OBJS_DIR)Sema_%)

EE_OBJS = $(ITO_OBJS) $(CLIST_OBJS) $(GSDRIVER_OBJS) $(GIFPIPE_OBJS) $(GIFPACKET_OBJS)  $(OPERATORS_OBJS)
EE_OBJS += $(GS_OBJS) $(FPUMATH_OBJS) $(VU0MATH_OBJS) $(TEXTUREMANAGER_OBJS) $(ITO_ASM_OBJS) $(PAD_OBJS) $(THREAD_OBJS) $(SEMA_OBJS)
EE_OBJS += $(VU0MATHASM_OBJS)

EE_LIB_DIR = $(LIBITO)/lib/
EE_LIB =  $(EE_LIB_DIR)libito.a

CLEAN_FILES = $(EE_LIB)

all: $(EE_OBJS_DIR) $(EE_OBJS) $(EE_LIB_DIR) $(EE_LIB)

$(OPERATORS_OBJS): Operators.cpp
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)%=%) $< -o $@

$(ITO_OBJS): Ito.cpp
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)Ito_%=%) $< -o $@

$(ITO_ASM_OBJS): ItoAsm.S
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)Ito_%=%) $< -o $@

$(VU0MATH_OBJS): Vu0Math.cpp
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)Vu0Math_%=%) $< -o $@

$(VU0MATHASM_OBJS): Vu0MathAsm.S
	$(EE_CC) $(EE_CFLAGS) -DF_$(*:$(EE_OBJS_DIR)Vu0MathAsm_%=%) $< -o $@

$(FPUMATH_OBJS): FpuMath.cpp
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)FpuMath_%=%) $< -o $@

$(TEXTUREMANAGER_OBJS): TextureManager.cpp
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)TextureManager_%=%) $< -o $@

$(GIFPACKET_OBJS): GifPacket.cpp
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)GifPacket_%=%) $< -o $@

$(GSDRIVER_OBJS): GsDriver.cpp
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)GsDriver_%=%) $< -o $@

$(GIFPIPE_OBJS): GifPipe.cpp
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)GifPipe_%=%) $< -o $@

$(GS_OBJS): Gs.cpp
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)Gs_%=%) $< -o $@

$(PAD_OBJS): Pad.cpp
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)Pad_%=%) $< -o $@

$(THREAD_OBJS): Thread.cpp
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)Thread_%=%) $< -o $@

$(SEMA_OBJS): Sema.cpp
	$(EE_CPP) $(EE_CXXFLAGS) -DF_$(*:$(EE_OBJS_DIR)Sema_%=%) $< -o $@

include $(LIBITO)/Makefile.prefab
