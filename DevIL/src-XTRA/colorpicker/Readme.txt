------------------------------------------------------------------------------------------------------------------------------------
Restrictions & freedoms pertaining to usage and redistribution of this software:
------------------------------------------------------------------------------------------------------------------------------------

  This software is 100% free. If you use this software (in part or in whole) you must credit the author. This software may not be
  re-distributed (in part or in whole) in a modified form without clear documentation on how to obtain a copy of the original
  work. You may not use this software to directly or indirectly cause harm to others. This software is provided as-is and without
  warrantee -- Use at your own risk. For more information, visit HTTP://www.FluidStudios.com/

  If you use this software in your own product, you MUST include this text file and LIB file with your product, or make sure that
  you point them at the Fluid Studios website so they may use it too. Share and enjoy.

  Copyright 2001, Fluid Studios & Paul Nettle. All rights reserved.

  Email: midnight@FluidStudios.com

------------------------------------------------------------------------------------------------------------------------------------
How to incorporate the color picker into your own apps, in three simple steps. :)
------------------------------------------------------------------------------------------------------------------------------------

1: Add the [included] lib file to your project

2: Use the following line to your source code to import the DLL's one-and-only function:

	__declspec( dllexport ) bool WINAPI FSColorPickerDoModal(unsigned int * currentColor, unsigned int * originalColor, const int initialExpansionState);

3: Have a look at how to call it:

	// Usage for the Fluid Studios Color Picker is trivial. There are two input colors:
	// 'currentColor' and 'originalColor'. The 'currentColor' represents the default color
	// for the color picker -- it starts off as if the user has selected this color.
	//
	// The other color, 'originalColor', is useful for times when you want to allow the user
	// to change a color. It shows this color in the bottom half of the "two-tone box." This
	// is useful for letting the user see the new color next to the original color they started
	// out with. If you set this value, then it's usually a good idea to set the 'currentColor'
	// to the same thing. This gives the user the imporession of "changing a color" as opposed to
	// "creating a color" (if that makes any sense :)
	//
	// Note that the dialog stores the previous color values in the registry. If you give it no
	// defaults, then the dialog will start with the values from the last time it was run. You
	// can force this by passing in NULL pointers. However, doing this means that the dialog has
	// nowhere to place the result (i.e. if you give it a NULL pointer, it can't tell you which
	// color the user selected.)
	//
	// So, you may optionally use the value 0x80000000 (high bit only) in either of these to tell
	// the color picker to start with its own defaults. On return, these colors will be set to
	// what the user selected.
	//
	// The final value specifies the initial state of the dialog, in terms of expansion. Here are
	// the values:
	//
	// 0 - use defaults
	// 1 - force expanded    show expansion button
	// 2 - force expanded    hide the expansion button
	// 3 - force compressed  show expansion button
	// 4 - force compressed  hide expansion button
	//
	// Return code is simple. true = "OK", false = "Cancel"

	unsigned int	currentColor = 0x80000000;
	unsigned int	originalColor = 0x80000000;

	bool	userClickedOK = FSColorPickerDoModal(&currentColor, &originalColor, 0);

	if (userClickedOK)
	{
		MessageBox(NULL, "Clicked OK", "What the user did", MB_OK);
	}
	else
	{
		MessageBox(NULL, "Clicked Cancel", "What the user did", MB_OK);
	}

