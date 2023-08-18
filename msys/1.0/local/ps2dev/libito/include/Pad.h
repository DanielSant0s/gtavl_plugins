
#include <tamtypes.h>
#include <kernel.h>
#include <libpad.h>

namespace Ito
{
	class Pad;
}

/** Wrapper class for libpad. 

	\par About
	This is a highlevel interface for libpad. This has been created
	to make it alot easier to work with pads on a simple level.

	If no other irx's are specificed on creation of the first Pad object,
	the Pad class loads the rom0:SIO2MAN and rom0:PADMAN modules and therefor
	its required to link with libpad.a (not libpadx.a!)

	\par Usage

	Create the Pad object, use Open(..) to open a pad on a specific
	port/slot. Then use ReadButtons() to read pad buttons/directional buttons.
	
	Please see samples in misc/pad directory for further usage.

*/
class Ito::Pad
{
	private:
		padButtonStatus	m_ButtonStatus ALIGNED(64);
		u8* m_PadBuffer;
	public:
		s32 m_Port; 
		s32 m_Slot;
	public:
		
		/** Constructor. */
		Pad(char* sio2manIrx = 0, char* padmanIrx = 0);
		
		/** Destructor. */
		~Pad();
		
		/** Open pad.
			\par port port to which the pad is connected (0 or 1)
			\par slot slot to which the pad is connected in a multitap, always 0 when not using multitap.
		*/
		
		s32 Open(s32 port, s32 slot);
		/** Close pad. */
		s32 Close();

		/** Get pad state. Please see libpad.h for pad states.*/
		u32 GetState();
		/** Read buttons. Can be used after opening the pad with Open(..)
		
		\par You can use the following libpad defines to read the pad
		PAD_LEFT, PAD_DOWN, PAD_RIGHT, PAD_UP, PAD_START
		PAD_R3, PAD_L3, PAD_SELECT, PAD_SQUARE, PAD_CROSS     
		PAD_CIRCLE, PAD_TRIANGLE, PAD_R1, PAD_L1, PAD_R2, PAD_L2

		\par Example of usage
		
		u32 buttons = pad.ReadButtons();

		if(buttons & PAD_CROSS)
		{	
			.. // Code to execute when cross on the pad was pressed.
		}


		*/
		u16 ReadButtons();
};
