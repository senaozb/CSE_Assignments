import java.util.Arrays;
import java.util.List;
import java.util.Scanner;
import java.lang.Math;
import java.awt.Dimension;
import java.awt.GridLayout;
import java.awt.FlowLayout;
import java.awt.Color;
import javax.swing.JButton;
import javax.swing.JFrame;
import javax.swing.JPanel;
import javax.swing.JLabel;
import javax.swing.JRadioButton;
import javax.swing.ButtonGroup;
import java.awt.event.ActionEvent;
import java.awt.event.ActionListener;
import java.lang.*;
import javax.swing.JTextField;
import java.io.File;
import java.util.Scanner;
import java.io.IOException;
import java.io.FileWriter;

/** PegSolitaire is the main class including all inner classes and methods. This class operates the functionality of the game*/
public class PegSolitaire
{
	/** Cell is an inner class and is used to store the info of each cell such as coordinates and type */
    public class Cell
    {
		/** Variable showing the type of cell (empty, peg, out) */
        public char type;
		/** Variable showing the row index */
        public int coorR;
		/** Variable showing the column index */
        public int coorC;

		public Cell()
		{
			coorR = -1;
			coorC = -1;
			type = 'x';
		}

        public Cell(int a, int b, char t)
        {
            coorR = a;
	        coorC = b;
	        type = t;
        }
    }

	/** Layout is an inner class and is used to create a GUI for the game and it operates the methods of GUI */
	public class Layout extends JPanel 
	{
		int SIZE = 50;
		JButton[][] array;
		JButton loadB, saveB;
		volatile int counter = 0;
		volatile int firstX, firstY, secondX, secondY;
		char[] inp = new char[4];
		
		JLabel text1, text2;
		JFrame frame;
		JPanel controlPanel, game;
		JRadioButton rb1, rb2, rb3, rb4, rb5;   

		/** It creates a grid layout and jbuttons. Then, it adds action listener components */
		public void setGame() 
		{
			JPanel panel = new JPanel();
			panel.setLayout(new GridLayout(row, col));
			panel.setPreferredSize(new Dimension(row * SIZE, col * SIZE));
			array = new JButton[row][col];
			
			for(int i = 0; i < row; i++)
			{
				for(int j = 0; j < col; j++)
				{
					Cell obj = index(i, j);
					String s = String.valueOf(obj.type);
					if(s.equals("o") == true || s.equals("e") == true) 
					{
						array[i][j] = new JButton(" ");
						if(s.equals("o") == true)
							array[i][j].setBorderPainted(false);
					}
					else
						array[i][j] = new JButton(s);
				}
			}
			
			for(int i = 0; i < row; i++) 
			{
				for(int j = 0; j < col; j++)
				{
					panel.add(array[i][j]);
					array[i][j].addActionListener(new ClickListener());
				}
			}

			game = panel;
		}

		/** This is the constructor of main frame. It creates all the buttons and adds all to the frame */
		public Layout()
		{
			//Create main frame with a grid layout
			frame = new JFrame("Peg Solitaire");
			frame.setSize(600,300);
			frame.setLayout(new GridLayout(1, 2));
			frame.setDefaultCloseOperation(JFrame.EXIT_ON_CLOSE);

			//Create a panel for control area
			controlPanel = new JPanel();
			controlPanel.setLayout(null);

			text1 = new JLabel("Welcome to Peg Solitaire");
			text1.setBounds(50,10,300,20);      
			controlPanel.add(text1);

			//Create radio buttons and adjust 
			rb1 = new JRadioButton("Type 1");  
			rb1.setBounds(100,50,100,30);     
			rb2 = new JRadioButton("Type 2");  
			rb2.setBounds(100,80,100,30);   
			rb3 = new JRadioButton("Type 3");  
			rb3.setBounds(100,110,100,30);    
			rb4 = new JRadioButton("Type 4"); 
			rb4.setBounds(100,140,100,30);       
			rb5 = new JRadioButton("Type 5"); 
			rb5.setBounds(100,170,100,30);      
			rb1.addActionListener(new RadioButtonListener());
			rb2.addActionListener(new RadioButtonListener());
			rb3.addActionListener(new RadioButtonListener());
			rb4.addActionListener(new RadioButtonListener());
			rb5.addActionListener(new RadioButtonListener());

			ButtonGroup G = new ButtonGroup();
			//Add radio buttons to the button group and panel
			G.add(rb1);  
			G.add(rb2);  
			G.add(rb3);  
			G.add(rb4);  
			G.add(rb5);   
			controlPanel.add(rb1);  
			controlPanel.add(rb2);  
			controlPanel.add(rb3);  
			controlPanel.add(rb4);  
			controlPanel.add(rb5);   

			//Create and add save and load buttons
			saveB = new JButton("S");
			loadB = new JButton("L");
			controlPanel.add(saveB);
			controlPanel.add(loadB);
			saveB.setBounds(80,220,30,30); 
			saveB.addActionListener(new SaveButtonListener());
			loadB.setBounds(140,220,30,30); 
			loadB.addActionListener(new LoadButtonListener());

			//Create a notification label 
			text2 = new JLabel("Choose only the board type");
			text2.setBounds(50,280,300,20);      
			controlPanel.add(text2);

			setGame();

			frame.add(game);
			frame.add(controlPanel);

			frame.pack();
			frame.setVisible(true);
		}

		/** This gets the input from the action listener and send it to movement validation method */
		public void move()
		{
			while(true)
			{
				if(fileFlag != 0)
					break;
				if(counter % 2 == 0 && counter != 0)
				{
					if((firstX == secondX) && (Math.abs(firstY-secondY) == 2))
					{
						if(firstY-secondY == 2)
							inp[3] = 'L';
						else
							inp[3] = 'R';

						inp[0] = (char) (firstY + '0');
						inp[1] = (char) (firstX + '0');
						inp[2] = '-';

						userIn = inp;
						if(movementValid() == true)
						{
							setUpdateFlag(1);
							break;
						}	
					}
					else if((firstY == secondY) && (Math.abs(firstX-secondX) == 2))
					{
						if(firstX-secondX == 2)
							inp[3] = 'U';
						else
							inp[3] = 'D';

						inp[0] = (char) (firstY + '0');
						inp[1] = (char) (firstX + '0');
						inp[2] = '-';

						userIn = inp;
						if(movementValid() == true)
						{
							setUpdateFlag(1);
							break;
						}
					}
				}
			}
		}

		/** This is a class for Peg Solitaire buttons to add action listener. 
		* Once it is clicked, the method stores the coordinates of the clicked button 
		*/
		class ClickListener implements ActionListener 
		{
			public void actionPerformed(ActionEvent e) 
			{
				counter++;
				Object obj = e.getSource();
				for(int i = 0; i < row; i++) 
				{
					for(int j = 0; j < col; j++)
					{
						JButton temp = (JButton)obj;
						if(temp == array[i][j])
						{
							if(counter % 2 != 0)
							{
								firstX = i;
								firstY = j;
							}
							else
							{
								secondX = i;
								secondY = j;
							}
						}
					}
				}
			}
		}

		/** This is a class for radio buttons to add action listener.
		* Once it is clicked, it changes the board type.
		*/
		class RadioButtonListener implements ActionListener
		{
			public void actionPerformed(ActionEvent e)
			{    
				if(rb1.isSelected() && typeFlag == 0)
				{    
					setTypeBoard(1);
					setTypeFlag(1);
				}    
				if(rb2.isSelected() && typeFlag == 0)
				{    
					setTypeBoard(2);
					setTypeFlag(1);
				}    
				if(rb3.isSelected() && typeFlag == 0)
				{    
					setTypeBoard(3);	
					setTypeFlag(1);	
				}    
				if(rb4.isSelected() && typeFlag == 0)
				{    
					setTypeBoard(4);	
					setTypeFlag(1);
				}    
				if(rb5.isSelected() && typeFlag == 0)
				{    
					setTypeBoard(5); 
					setTypeFlag(1);	
				}    
			}    
		}

		/** This is a class for the save button to add action listener.
		* Once it is clicked, it changes file flag.
		*/
		class SaveButtonListener implements ActionListener
		{
			public void actionPerformed(ActionEvent e)
			{    
				setFileFlag(1);
			}    
		}

		/** This is a class for the load button to add action listener.
		* Once it is clicked, it changes file flag.
		*/
		class LoadButtonListener implements ActionListener
		{
			public void actionPerformed(ActionEvent e)
			{    
				setFileFlag(2);
			}    
		}
	}

	/** A flag to check if the game is ended */
	public int flag = 0;
	public Cell[][] arr;
	public int row = 7; //Default values
	public int col = 7; //Default values
    private int chosenType = 1;
    private char[] userIn;  
	
	public volatile int typeFlag = 0;
	public volatile int updateFlag = 0;
	public volatile int fileFlag = 0;
	public Layout gui;

	public PegSolitaire()
	{
		//Type 1 is created at first as default
		create();
		gui = new Layout();
	}

	public int getTypeBoard() {return chosenType;}
	public void setTypeBoard(int a) {chosenType = a;}
	public void setTypeFlag(int a) {typeFlag = a;}
	public void setUpdateFlag(int a) {updateFlag = a;}
	public void setFileFlag(int a) {fileFlag = a;}
	public Cell index(int a, int b) {return arr[a][b];}

	private void type1()
	{
		Cell[][] temp = {{new Cell(0, 0,  'o'), new Cell(0, 1,  'o'), new Cell(0, 2,  'p'), new Cell(0, 3,  'p'), new Cell(0, 4,  'p'), new Cell(0, 5,  'o'), new Cell(0, 6,  'o')},
						{new Cell(1, 0,  'o'), new Cell(1, 1,  'p'), new Cell(1, 2,  'p'), new Cell(1, 3,  'p'), new Cell(1, 4,  'p'), new Cell(1, 5,  'p'), new Cell(1, 6,  'o')},
						{new Cell(2, 0,  'p'), new Cell(2, 1,  'p'), new Cell(2, 2,  'p'), new Cell(2, 3,  'e'), new Cell(2, 4,  'p'), new Cell(2, 5,  'p'), new Cell(2, 6,  'p')},
						{new Cell(3, 0,  'p'), new Cell(3, 1,  'p'), new Cell(3, 2,  'p'), new Cell(3, 3,  'p'), new Cell(3, 4,  'p'), new Cell(3, 5,  'p'), new Cell(3, 6,  'p')},
						{new Cell(4, 0,  'p'), new Cell(4, 1,  'p'), new Cell(4, 2,  'p'), new Cell(4, 3,  'p'), new Cell(4, 4,  'p'), new Cell(4, 5,  'p'), new Cell(4, 6,  'p')},
						{new Cell(5, 0,  'o'), new Cell(5, 1,  'p'), new Cell(5, 2,  'p'), new Cell(5, 3,  'p'), new Cell(5, 4,  'p'), new Cell(5, 5,  'p'), new Cell(5, 6,  'o')},
						{new Cell(6, 0,  'o'), new Cell(6, 1,  'o'), new Cell(6, 2,  'p'), new Cell(6, 3,  'p'), new Cell(6, 4,  'p'), new Cell(6, 5,  'o'), new Cell(6, 6,  'o')}};

		this.arr = temp;
		col = arr[0].length;
		row = arr.length;
	}
	private void type2()
	{
		Cell[][] temp = {{new Cell(0, 0,  'o'), new Cell(0, 1,  'o'), new Cell(0, 2,  'o'), new Cell(0, 3,  'p'), new Cell(0, 4,  'p'), new Cell(0, 5,  'p'), new Cell(0, 6,  'o'), new Cell(0, 7,  'o'), new Cell(0, 8,  'o')},
						{new Cell(1, 0,  'o'), new Cell(1, 1,  'o'), new Cell(1, 2,  'o'), new Cell(1, 3,  'p'), new Cell(1, 4,  'p'), new Cell(1, 5,  'p'), new Cell(1, 6,  'o'), new Cell(1, 7,  'o'), new Cell(1, 8,  'o')},
						{new Cell(2, 0,  'o'), new Cell(2, 1,  'o'), new Cell(2, 2,  'o'), new Cell(2, 3,  'p'), new Cell(2, 4,  'p'), new Cell(2, 5,  'p'), new Cell(2, 6,  'o'), new Cell(2, 7,  'o'), new Cell(2, 8,  'o')},
						{new Cell(3, 0,  'p'), new Cell(3, 1,  'p'), new Cell(3, 2,  'p'), new Cell(3, 3,  'p'), new Cell(3, 4,  'p'), new Cell(3, 5,  'p'), new Cell(3, 6,  'p'), new Cell(3, 7,  'p'), new Cell(3, 8,  'p')},
						{new Cell(4, 0,  'p'), new Cell(4, 1,  'p'), new Cell(4, 2,  'p'), new Cell(4, 3,  'p'), new Cell(4, 4,  'e'), new Cell(4, 5,  'p'), new Cell(4, 6,  'p'), new Cell(4, 7,  'p'), new Cell(4, 8,  'p')},
						{new Cell(5, 0,  'p'), new Cell(5, 1,  'p'), new Cell(5, 2,  'p'), new Cell(5, 3,  'p'), new Cell(5, 4,  'p'), new Cell(5, 5,  'p'), new Cell(5, 6,  'p'), new Cell(5, 7,  'p'), new Cell(5, 8,  'p')},
						{new Cell(6, 0,  'o'), new Cell(6, 1,  'o'), new Cell(6, 2,  'o'), new Cell(6, 3,  'p'), new Cell(6, 4,  'p'), new Cell(6, 5,  'p'), new Cell(6, 6,  'o'), new Cell(6, 7,  'o'), new Cell(6, 8,  'o')},
						{new Cell(7, 0,  'o'), new Cell(7, 1,  'o'), new Cell(7, 2,  'o'), new Cell(7, 3,  'p'), new Cell(7, 4,  'p'), new Cell(7, 5,  'p'), new Cell(7, 6,  'o'), new Cell(7, 7,  'o'), new Cell(7, 8,  'o')},
						{new Cell(8, 0,  'o'), new Cell(8, 1,  'o'), new Cell(8, 2,  'o'), new Cell(8, 3,  'p'), new Cell(8, 4,  'p'), new Cell(8, 5,  'p'), new Cell(8, 6,  'o'), new Cell(8, 7,  'o'), new Cell(8, 8,  'o')}};

		this.arr = temp;
		col = arr[0].length;
		row = arr.length;
	}
	private void type3()
	{
		Cell[][] temp = {{new Cell(0, 0,  'o'), new Cell(0, 1,  'o'), new Cell(0, 2,  'p'), new Cell(0, 3,  'p'), new Cell(0, 4,  'p'), new Cell(0, 5,  'o'), new Cell(0, 6,  'o'), new Cell(0, 7,  'o')},
						{new Cell(1, 0,  'o'), new Cell(1, 1,  'o'), new Cell(1, 2,  'p'), new Cell(1, 3,  'p'), new Cell(1, 4,  'p'), new Cell(1, 5,  'o'), new Cell(1, 6,  'o'), new Cell(1, 7,  'o')},
						{new Cell(2, 0,  'o'), new Cell(2, 1,  'o'), new Cell(2, 2,  'p'), new Cell(2, 3,  'p'), new Cell(2, 4,  'p'), new Cell(2, 5,  'o'), new Cell(2, 6,  'o'), new Cell(2, 7,  'o')},
						{new Cell(3, 0,  'p'), new Cell(3, 1,  'p'), new Cell(3, 2,  'p'), new Cell(3, 3,  'p'), new Cell(3, 4,  'p'), new Cell(3, 5,  'p'), new Cell(3, 6,  'p'), new Cell(3, 7,  'p')},
						{new Cell(4, 0,  'p'), new Cell(4, 1,  'p'), new Cell(4, 2,  'p'), new Cell(4, 3,  'e'), new Cell(4, 4,  'p'), new Cell(4, 5,  'p'), new Cell(4, 6,  'p'), new Cell(4, 7,  'p')},
						{new Cell(5, 0,  'p'), new Cell(5, 1,  'p'), new Cell(5, 2,  'p'), new Cell(5, 3,  'p'), new Cell(5, 4,  'p'), new Cell(5, 5,  'p'), new Cell(5, 6,  'p'), new Cell(5, 7,  'p')},
						{new Cell(6, 0,  'o'), new Cell(6, 1,  'o'), new Cell(6, 2,  'p'), new Cell(6, 3,  'p'), new Cell(6, 4,  'p'), new Cell(6, 5,  'o'), new Cell(6, 6,  'o'), new Cell(6, 7,  'o')},
						{new Cell(7, 0,  'o'), new Cell(7, 1,  'o'), new Cell(7, 2,  'p'), new Cell(7, 3,  'p'), new Cell(7, 4,  'p'), new Cell(7, 5,  'o'), new Cell(7, 6,  'o'), new Cell(7, 7,  'o')}};

		this.arr = temp;
		col = arr[0].length;
		row = arr.length;
	}
	private void type4()
	{
		Cell[][] temp = {{new Cell(0, 0,  'o'), new Cell(0, 1,  'o'), new Cell(0, 2,  'p'), new Cell(0, 3,  'p'), new Cell(0, 4,  'p'), new Cell(0, 5,  'o'), new Cell(0, 6,  'o')},
						{new Cell(1, 0,  'o'), new Cell(1, 1,  'o'), new Cell(1, 2,  'p'), new Cell(1, 3,  'p'), new Cell(1, 4,  'p'), new Cell(1, 5,  'o'), new Cell(1, 6,  'o')},
						{new Cell(2, 0,  'p'), new Cell(2, 1,  'p'), new Cell(2, 2,  'p'), new Cell(2, 3,  'p'), new Cell(2, 4,  'p'), new Cell(2, 5,  'p'), new Cell(2, 6,  'p')}, 
						{new Cell(3, 0,  'p'), new Cell(3, 1,  'p'), new Cell(3, 2,  'p'), new Cell(3, 3,  'e'), new Cell(3, 4,  'p'), new Cell(3, 5,  'p'), new Cell(3, 6,  'p')},
						{new Cell(4, 0,  'p'), new Cell(4, 1,  'p'), new Cell(4, 2,  'p'), new Cell(4, 3,  'p'), new Cell(4, 4,  'p'), new Cell(4, 5,  'p'), new Cell(4, 6,  'p')},
						{new Cell(5, 0,  'o'), new Cell(5, 1,  'o'), new Cell(5, 2,  'p'), new Cell(5, 3,  'p'), new Cell(5, 4,  'p'), new Cell(5, 5,  'o'), new Cell(5, 6,  'o')},
						{new Cell(6, 0,  'o'), new Cell(6, 1,  'o'), new Cell(6, 2,  'p'), new Cell(6, 3,  'p'), new Cell(6, 4,  'p'), new Cell(6, 5,  'o'), new Cell(6, 6,  'o')}};

		this.arr = temp;
		col = arr[0].length;
		row = arr.length;
	}
	private void type5()
	{
		Cell[][] temp = {{new Cell(0, 0,  'o'), new Cell(0, 1,  'o'), new Cell(0, 2,  'o'), new Cell(0, 3,  'o'), new Cell(0, 4,  'p'), new Cell(0, 5,  'o'), new Cell(0, 6,  'o'), new Cell(0, 7,  'o'), new Cell(0, 8,  'o')},
						{new Cell(1, 0,  'o'), new Cell(1, 1,  'o'), new Cell(1, 2,  'o'), new Cell(1, 3,  'p'), new Cell(1, 4,  'p'), new Cell(1, 5,  'p'), new Cell(1, 6,  'o'), new Cell(1, 7,  'o'), new Cell(1, 8,  'o')},
						{new Cell(2, 0,  'o'), new Cell(2, 1,  'o'), new Cell(2, 2,  'p'), new Cell(2, 3,  'p'), new Cell(2, 4,  'p'), new Cell(2, 5,  'p'), new Cell(2, 6,  'p'), new Cell(2, 7,  'o'), new Cell(2, 8,  'o')},
						{new Cell(3, 0,  'o'), new Cell(3, 1,  'p'), new Cell(3, 2,  'p'), new Cell(3, 3,  'p'), new Cell(3, 4,  'p'), new Cell(3, 5,  'p'), new Cell(3, 6,  'p'), new Cell(3, 7,  'p'), new Cell(3, 8,  'o')},
						{new Cell(4, 0,  'p'), new Cell(4, 1,  'p'), new Cell(4, 2,  'p'), new Cell(4, 3,  'p'), new Cell(4, 4,  'e'), new Cell(4, 5,  'p'), new Cell(4, 6,  'p'), new Cell(4, 7,  'p'), new Cell(4, 8,  'p')}, 
						{new Cell(5, 0,  'o'), new Cell(5, 1,  'p'), new Cell(5, 2,  'p'), new Cell(5, 3,  'p'), new Cell(5, 4,  'p'), new Cell(5, 5,  'p'), new Cell(5, 6,  'p'), new Cell(5, 7,  'p'), new Cell(5, 8,  'o')},
						{new Cell(6, 0,  'o'), new Cell(6, 1,  'o'), new Cell(6, 2,  'p'), new Cell(6, 3,  'p'), new Cell(6, 4,  'p'), new Cell(6, 5,  'p'), new Cell(6, 6,  'p'), new Cell(6, 7,  'o'), new Cell(6, 8,  'o')},
						{new Cell(7, 0,  'o'), new Cell(7, 1,  'o'), new Cell(7, 2,  'o'), new Cell(7, 3,  'p'), new Cell(7, 4,  'p'), new Cell(7, 5,  'p'), new Cell(7, 6,  'o'), new Cell(7, 7,  'o'), new Cell(7, 8,  'o')},
						{new Cell(8, 0,  'o'), new Cell(8, 1,  'o'), new Cell(8, 2,  'o'), new Cell(8, 3,  'o'), new Cell(8, 4,  'p'), new Cell(8, 5,  'o'), new Cell(8, 6,  'o'), new Cell(8, 7,  'o'), new Cell(8, 8,  'o')}};

		this.arr = temp;
		col = arr[0].length;
		row = arr.length;
	}

	/** This calls the right method to create an array */
	public void create()
	{
		switch(chosenType)
		{
			//Create the game board array
			case 1:
				type1();
				break;
			case 2:
				type2();
				break;
			case 3:
				type3();
				break;
			case 4:
				type4();
				break;
			case 5:
				type5();
				break;
		}
	}

	/** This starts the game by creating the gui and calling the playUser method */
	public void createGUI()
	{
		while(true)
		{
			if(typeFlag == 1)
			{
				gui.frame.getContentPane().removeAll();
				gui.frame.getContentPane().invalidate();

				create();
				
				gui.setGame();
				gui.frame.getContentPane().add(gui.game);
				gui.frame.getContentPane().add(gui.controlPanel);
				gui.frame.getContentPane().revalidate();

				gui.text2.setText(" ");
				
				setTypeFlag(0);
				break;
			}
		}

		flag = 0;
		gui.counter = 0;
		playUser();
	}

	/** This allows the user to play and operates the file operations*/
	public void playUser()
	{
		do
		{
			if(fileFlag == 0)
			{
				gui.move();
				if(updateFlag == 1)
				{
					gui.frame.getContentPane().removeAll();
					gui.frame.getContentPane().invalidate();
						
					update();
					gui.setGame();
					gui.frame.getContentPane().add(gui.game);
					gui.frame.getContentPane().add(gui.controlPanel);
					gui.frame.getContentPane().revalidate();

					gui.text2.setText(" ");

					flag = checkGame();
				}
			}
			
			if(fileFlag == 1)
			{
				save("game.txt");
				break;
			}
			if(fileFlag == 2)
			{
				boolean res = load("game.txt");
				if(res == false)
					setFileFlag(0);
				else
					break;
			}

		}while(flag == 0);

		if(fileFlag == 0)
		{
			printRes();
			wait(2000);
			gui.text2.setText("Choose only the board type");
		}
		else if(fileFlag == 1)
		{
			wait(2000);
			gui.text2.setText("Choose only the board type");
		}
		else if(fileFlag == 2)
		{
			gui.frame.getContentPane().removeAll();
			gui.frame.getContentPane().invalidate();
						
			gui.setGame();
			gui.frame.getContentPane().add(gui.game);
			gui.frame.getContentPane().add(gui.controlPanel);
			gui.frame.getContentPane().revalidate();

			gui.text2.setText("Successfully loaded");
			wait(1000);
			gui.text2.setText(" ");
			
			setFileFlag(0);
			flag = 0;
			gui.counter = 0;
			playUser();
		}

		setFileFlag(0);
		createGUI();
	}

	public static void wait(int ms)
	{
		try
		{
			Thread.sleep(ms);
		}
		catch(InterruptedException ex)
		{
			Thread.currentThread().interrupt();
		}
	}

	/** This checks the validation of the given movement looking at the game board
	* @return if it is valid then return true
	*/
	private boolean movementValid()
	{
		int row = (int)(userIn[1]) - 48;
		int col = (int)(userIn[0]) - 48;
		char dir = userIn[3];
		int max_r = arr.length - 1;
		int max_c = arr[0].length - 1;
		
		if (arr[row][col].type ==  'o' || arr[row][col].type ==  'e')
			return false;
		
		//If there is no peg and empty cell after the chosen peg then return 0
		if (dir == 'R')
		{
			if (max_c - col < 2)
				return false;
			if (!(arr[row][col+1].type ==  'p' && arr[row][col+2].type ==  'e'))
				return false;
		}
		else if (dir == 'L')
		{
			if (col < 2)
				return false;
			if (!(arr[row][col-1].type ==  'p' && arr[row][col-2].type ==  'e'))
				return false;
		}
		else if (dir == 'U')
		{
			if (row < 2)
				return false;
			if (!(arr[row-1][col].type ==  'p' && arr[row-2][col].type ==  'e'))
				return false;
		}
		else if (dir == 'D')
		{
			if (max_r - row < 2)
				return false;
			if (!(arr[row+1][col].type ==  'p' && arr[row+2][col].type ==  'e'))
				return false;
		}
		
		return true;
	}

	/** This changes the position of the peg and remove one peg */
	private void update()
	{
		int row = (int)(userIn[1]) - 48;
		int col = (int)(userIn[0]) - 48;
		char dir = userIn[3];

			switch (dir)
			{
				case 'R':
					arr[row][col].type =  'e';
					arr[row][col+1].type =  'e';
					arr[row][col+2].type =  'p';
					break;
				case 'L':
					arr[row][col].type =  'e';
					arr[row][col-1].type =  'e';
					arr[row][col-2].type =  'p';
					break;
				case 'U':
					arr[row][col].type =  'e';
					arr[row-1][col].type =  'e';
					arr[row-2][col].type =  'p';
					break;
				case 'D':
					arr[row][col].type =  'e';
					arr[row+1][col].type =  'e';
					arr[row+2][col].type =  'p';
					break;
			}

		setUpdateFlag(0);
	}

	/**  This checks the immovable situations for being stuck
	* @return count 
	*/
	private int checkFull(int indR, int indC, int type, int locType)
	{
			//locType indicates the location of peg to prevent the memory errors(e.g. 1.column or the last row)
			int count = 0, fl = 0;

			for (int j = 0; j < col; j++) //Full row control
			{
				if(arr[indR][j].type ==  'p')
					++count;
				else if(arr[indR][j].type ==  'e') //If there is an empty area then reset counter
				{
					count = 0;
					fl = 1;
					break;
				}
			}
			
			if(fl == 0)
			{
				//Checks if up and down rows are empty, if not so returns 0
				
					for (int k = 0; k < col; k++)
					{
						if(locType == 0 || locType == 3 || locType == 4)
						{
							if(arr[indR][k].type ==  'p' && (arr[indR-1][k].type ==  'p' || arr[indR+1][k].type ==  'p'))
								return 0;  
						}
						else if(locType == 1)
						{
							if(arr[indR][k].type ==  'p' && arr[indR+1][k].type ==  'p')
								return 0;  
						}
						else if(locType == 2)
						{
							if(arr[indR][k].type ==  'p' && arr[indR-1][k].type ==  'p')
								return 0;  
						}
					}
					return count;
				
			}
			
			count = 0;
		
			for (int i = 0; i < row; i++) //Full column control
			{
				if(arr[i][indC].type ==  'p')
					++count;
				else if(arr[i][indC].type ==  'e')
					return 0;
			}
			for (int l = 0; l < row; l++) //Checks if the right and left columns are empty
			{
				if(locType == 0 || locType == 1 || locType == 2)
				{
					if(arr[l][indC].type ==  'p' && (arr[l][indC-1].type ==  'p' || arr[l][indC+1].type ==  'p'))
						return 0;  
				} 
				else if(locType == 3)
				{
					if(arr[l][indC].type ==  'p' && arr[l][indC+1].type ==  'p')
						return 0;  
				}
				else if(locType == 4)
				{
					if(arr[l][indC].type ==  'p' && arr[l][indC-1].type ==  'p')
						return 0;  
				}

			}
		
		return count;
	}

	/** This checks if the game is finished or not 
	* @return count
	*/
	private int checkGame()
	{
		int count = 0;

		for (int i = 0; i < row; i++)
		{
			for (int j = 0; j < col; j++)
			{
				if (arr[i][j].type ==  'p') //Check around the peg if there are other pegs
				{   
					if (i != 0 && j != 0 && i != row-1 && j != col-1)
					{
						if (!(arr[i][j+1].type !=  'p' && arr[i][j-1].type !=  'p' 
							&& arr[i+1][j].type !=  'p' && arr[i-1][j].type !=  'p'))
						{
							if (checkFull(i, j, 0, 0) == 0)
								return 0;
							else
								++count;
						}
						else    
							++count;
					}
					else if (i == 0)
					{
						if (!(arr[i][j+1].type !=  'p' && arr[i][j-1].type !=  'p' 
							&& arr[i+1][j].type !=  'p'))
						{
							if (checkFull(i, j, 0, 1) == 0)
								return 0;
							else
								++count;
						}
						else    
							++count;
					}
					else if (i == row-1)
					{
						if (!(arr[i][j+1].type !=  'p' && arr[i][j-1].type !=  'p' 
							&& arr[i-1][j].type !=  'p'))
						{
							if (checkFull(i, j, 0, 2) == 0)
								return 0;
							else
								++count;
						}
						else    
							++count;
					}
					else if (j == 0)
					{
						if (!(arr[i][j+1].type !=  'p' && arr[i+1][j].type !=  'p' 
							&& arr[i-1][j].type !=  'p'))
						{
							if (checkFull(i, j, 0, 3) == 0)
								return 0;
							else
								++count;
						}
						else    
							++count;
					}
					else if (j == col-1)
					{
						if (!(arr[i][j-1].type !=  'p' && arr[i+1][j].type !=  'p' 
							&& arr[i-1][j].type !=  'p'))
						{
							if (checkFull(i, j, 0, 4) == 0)
								return 0;
							else
								++count;
						}
						else    
							++count;
					}
				}
			}  
		}
		return count;
	}

	public void printRes()  
	{
		if (flag == 1)
			gui.text2.setText("Final Score: 1. You won!");
		else
			gui.text2.setText("Final Score: " + flag);
	}

	/** @return if the game is successfully loaded, then return true */
	public boolean load(String fileName)
	{
		try 
        {
			int sizeStr = 0, l = 0, c = 0;
            File obj = new File(fileName);
            Scanner reader = new Scanner(obj);
			Cell[][] t = arr;

			while (reader.hasNextLine()) 
			{
				String data = reader.nextLine();
				sizeStr = data.length();
				if(c == 0) //Create a N*N array
				{
					if(sizeStr <= 5)
					{
						gui.text2.setText("Size should be larger than 5!");
						reader.close();
						return false;
					}
					else
					{
						t = new Cell[sizeStr][sizeStr];
						for(int m = 0; m < sizeStr; m++)
						{
							for(int n = 0; n < sizeStr; n++)
							{
								Cell tempObj = new Cell(m, n, 'x');
								t[m][n] = tempObj;
							}
						}
					}
				}

				char[] dataArr = data.toCharArray();
				
				for(int k = 0; k < sizeStr; k++)
				{
					t[l][k].type = dataArr[k];
				}
				++l;
				++c;
			}
            
			arr = t;
			row = sizeStr;
			col = sizeStr;
			reader.close();
        }
        catch (IOException e) 
        {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }	
		return true;
	}

	public void save(String fileName)
	{
		try 
        {
            File obj = new File(fileName);
            FileWriter ioFile;
            ioFile = new FileWriter(fileName);

			for(int i = 0; i < row; i++)
			{
				for(int j = 0; j < col; j++)
					ioFile.write(arr[i][j].type);
				ioFile.write("\n");
			}
            ioFile.close();
            gui.text2.setText("Successfully saved");
        }
        catch (IOException e) 
        {
            System.out.println("An error occurred.");
            e.printStackTrace();
        }	
	}
}