How to create a bitmap font
---------------------------


The easiest way to do this is using the application "Bitmap Font Builder" available at http://www.lmnopc.com/bitmapfontbuilder.

Steps
-----

1.	Load "Bitmap Font Builder"

2.	From the 'Character Set' menu select 'Full ASCII Character Set'

3.	From the 'Font Smoothing' menu select 'Enable'

4.	Choose your desired Font from the pulldown selection.

5.	Click on the alignment icon, and choose 'Left'

6.	Choose the desired size of your texture.

	(Note: If you want your final bitmap to be below 512x512, 
	then I would suggest selecting 512x512, then scaling it down
	later with another tool like 'GIMP' to 256x256, as this will
	give better final results)

7.	From the 'File' menu select 'Save 32-bit TGA' and choose your
	filename then click 'Save'

8.	If you have saved a bitmap larger than your final desired bitmap size,
	then make sure you select the final size of your bitmap now.

9.	From the 'File' menu select 'Save Font Widths (Byte Format)' and
	choose your filename then click 'Save'

10.	Close "Bitmap Font Builder"

11.	If the desired final size of your bitmap is smaller (ie: 256x256) than the bitmap
	you saved with "Bitmap Font Builder" (ie: 512x512) then scale the bitmap with
	an app like "GIMP" (www.gimp.org), and make sure to save the scaled bitmap as
	32-bit TGA, uncompressed.

12.	Run tga2fnt (either win32-gui or console version).

13.	Specify the filenames as prompted.

14.	Specify the grid-size and character pixel sizes as prompted.

	(Note: If you created your bitmap using bitmap font builder, then the grid sizes
	will always be 16x16, and the character sizes will be the size of the bitmap
	divided by 16).

Done !