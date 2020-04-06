// This file is part of www.nand2tetris.org
// and the book "The Elements of Computing Systems"
// by Nisan and Schocken, MIT Press.
// File name: projects/04/Fill.asm

// Runs an infinite loop that listens to the keyboard input.
// When a key is pressed (any key), the program blackens the screen,
// i.e. writes "black" in every pixel;
// the screen should remain fully black as long as the key is pressed. 
// When no key is pressed, the program clears the screen, i.e. writes
// "white" in every pixel;
// the screen should remain fully clear as long as no key is pressed.

// Put your code here.

// blocks = 8192
// LOOP :
//   if KBD != 0 goto FILL
//   reset whole SCREEN
//   goto LOOP
//   
// FILL :
//   set whole SCREEN
//   goto LOOP

@8192
D = A
@blocks
M = D    // blocks = 8192

(LOOP)
	@KBD
	D = M
	@FILL
	D ; JNE // if KBD != 0 goto FILL

	@n_block
	M = 0 // n_block = 0

	(EMPTYSCREEN) // reset whole SCREEN
		@n_block
		D = M
		@blocks
		D = D - M
		@LOOP
		D ; JEQ // if n_block - blocks == 0 goto LOOP

		@n_block
		D = M // D = n_block
		@SCREEN
		A = A + D
		M = 0 // reset SCREEN[n_block]

		@n_block
		M = M + 1 // n_block = n_block + 1

		@EMPTYSCREEN
		0 ; JMP // goto EMPTYSCREEN

	@LOOP
	0 ; JMP // goto LOOP

(FILL)
	@SCREEN
	M = -1 // set SCREEN

	@n_block
	M = 0 // n_block = 0

	(FILLSCREEN) // set whole SCREEN
		@n_block
		D = M
		@blocks
		D = D - M
		@LOOP
		D ; JEQ // if n_block - blocks == 0 goto LOOP

		@n_block
		D = M
		@SCREEN
		A = A + D
		M = -1 // set SCREEN[n_block]

		@n_block
		M = M + 1 // n_block = n_block + 1

		@FILLSCREEN
		0 ; JMP // goto FILLSCREEN

	@LOOP
	0 ; JMP // goto LOOP