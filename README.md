# Snake Game 🐍


## 1. Download and Extract
- Download the `.zip` file from the repository.
- Extract it to a location of your choice:  
  - **Recommended**: Use either **WSL** (Windows Subsystem for Linux) or a **Virtual Machine (VM)**, depending on your operating system.

## 2. Prerequisites
Before running the game, ensure the following are installed and set up on your system:   
- **RISC-V toolchain**  
- **USB-Blaster** 
- **JTAG Software** 
- **dtekv-tools** 

## 3. Running the Game
1. Connect the board using the USB-Blaster and JTAG Software.
2. The extracted `.zip` should include a runnable file named `main.bin`.  
3. To run the game:  
   - Open a terminal and navigate to your `~/dtekv-tools` folder.
   - Execute the following command (replace `"your/path"` with the path to your `main.bin` file):  
     ```bash
     ./dtekv-run ~/"your/path"/main.bin
     ```

## 4. Hardware Requirements
- The game requires two external buttons connected to:  
  - **GPIO0**: To turn left.  
  - **GPIO1**: To turn right.

## 5. If the `.bin` File is Missing
- If the `main.bin` file is not included in the extracted folder, you will need to compile the game manually:  
  1. Navigate to the `snake/scr` folder in your terminal.  
  2. Run the following command:  
     ```bash
     make
     ```

You're all set to play the game! 🎮
