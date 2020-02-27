

/*
		  Created 02/15/2014
		  Author: Noreddine Kessa
		  Purpose: To provide a frame work for managing program options, in an organized and reusable manner. 
		  Note that you can have as many depth of nesting as you like, 
		  you can have menus nested within menus within menus............. 
 */
//===============================================================================
//few definitions used in these programs

#define PASS 0
#define FAIL 1
#define NOT_PERFORMED 2
#define NOT_TEST 3
#define ENTER 0XD
#define UP 0X55
#define up 0X75
#define DOWN 0X44
#define down 0X64
#define LEFT 0X4C
#define left 0X6C
#define back 0X62
#define BACK 0x42
#define RIGHT 0X52
#define right 0X72
#define TERM_CLEAR_SCREEN "1B[J"
#define TERM_NEW_LINE (char)0XC
#define TERM_CARRIAGE_RETURN  (char)0xD
#define END_STRING (char)0X00 //String terminator
#define ESC 0X1B

//============================================================================
class Test{
	//Purpose: Stores data related to any particular option

private: 

public:

	String Name ; 
	char Letter;
	int Number; 
	int (* Function)() ; 
	int Status ; 

	//constructors

	Test(){
	}

	Test(String Name_ , char Letter_ , int Number_ , int ( * Function_)(),int Initial_Status_){
		//Serial.println("Test Constructor ") ; 
		Name = Name_ ; 
		Letter = Letter_ ;
		Number = Number_ ;
		Function = Function_ ; 
		Status = Initial_Status_ ; 
	}


	//This function runs the test associated with option.
	//The status of the option is stored after the program is ran.
	void runTest(){
		Status =  (*Function)();

	}
};

//=================================================================================
class Screen{
	//this class groups the functions that are related to managing the screen

public:

	static void clearScreen(){
		Serial.write(27); //ESC command 
		Serial.print("[2J"); // clear screen
		Serial.write(27); //ESC command 
		Serial.print("[3j"); // clear screen

	}

	static void eraseBelow(){
		Serial.write(27); //ESC Comand 
		Serial.print("[J"); // Command to rerase below the cursor
		Serial.flush();
	}

	static void cursorHome(){
		Serial.write(27); //ESC Comand 
		Serial.print("[H"); // Cursor to home
		Serial.flush();
	}

};

//==================================================================================
// The test manager class is useful for managing tests in a well structure manner  
class TestsManager{

private: 

public: 
	static const int MAX_NUM_TEST = 30 ; 
	int  LastTest ; 
	int CurrentTest;
	Test * Tests[10];

	TestsManager(){
		LastTest=-1;
		CurrentTest=0;
	}



	String Status_Num_to_String(int status_){
		if (status_ > 3)
			return "UNKNOWN_STATUS" ;
		String Statuss[]={
				"PASS         " 
				,  "FAIL         " 
				,  "NOT_PERFORMED" 
				,  "             " };   //not that when index = 3 the status string is empty, this is usefull when using the test manager for none testing function
		return Statuss[status_] ;
	}


	void addTest(String Name_ , char Letter_ , int Number_ , int (*Function_)(),int Initial_Status_){
		LastTest ++ ; 
		//CurrentTest =1 ;
		Tests[LastTest] = new Test( Name_ ,  Letter_ ,  Number_ ,  *Function_, Initial_Status_);
	}


	int readinput(){
		//This function listens for the user to send one character. 
		//Then it returns the value that corresponds to that entry.
		//This function also checks if the UP and DOWN arrows are pressed

		int incoming[] = {0, 0 , 0} ;
		int bufferSize =0 , i ; 
		incoming[0] = Serial.read();

		while((i= Serial.available()) <=0){;} //wait for the keyboard to be pressed
		incoming[0]=Serial.read(); //read the first character in the buffer
		delay(100); //delay to allow shifting the next character if available
		if (Serial.available()>=1){ //at this point a good chance that either UP or DOWN keys have been pressed
			incoming[1]= Serial.read(); 
			incoming[2]=Serial.read();
			//Serial.print("Three:");
			if ( (incoming[0]==27) && (incoming[1]==91) && (incoming[2]==65))
				return up ; 
			if ( (incoming[0]==27) && (incoming[1]==91) && (incoming[2]==66))
				return down ;  

		}else {
			//Serial.print("One:");
			return incoming[0] ; // only one character have been sent by the keyboard, return it
		}


	}
	


	void printMain(){

		String strTemp = " ";
		int i =0 ;

		strTemp+="Welcome to the Options Manager...." ;
		//Serial.println("Welcome to the Test Manager...." );
		strTemp+= TERM_CARRIAGE_RETURN ;
		strTemp+=TERM_NEW_LINE;
		strTemp+= TERM_CARRIAGE_RETURN ;
		strTemp+=TERM_NEW_LINE;
		strTemp+= "==========#==========#=============#";
		strTemp+= TERM_CARRIAGE_RETURN ;
		strTemp+=TERM_NEW_LINE;
		//Serial.write(TERM_NEW_LINE);

		//Print table header 
		strTemp+="Test Name #" ;
		//Serial.print("Test Name ") ; 
		// "=========="
		strTemp+= "Cursor    #";
		//Serial.print("Cursor    ") ; 
		// "=========="
		strTemp+= "Result       #";
		strTemp+= TERM_CARRIAGE_RETURN;
		strTemp+=TERM_NEW_LINE;
		//Serial.println("Result       ") ; 
		// "==============="
		strTemp+= "==========#==========#=============#";
		strTemp+= TERM_CARRIAGE_RETURN;
		strTemp+=TERM_NEW_LINE;
		strTemp+= "          #          #             #";
		strTemp+= TERM_CARRIAGE_RETURN ;
		strTemp+=TERM_NEW_LINE;
		//Serial.println("===================================") ; 
		//Serial.write(TERM_NEW_LINE);

		//Print each option and its results
		for (i=0 ; i<=LastTest ; i++){
			strTemp += Tests[i]->Name ;
			strTemp += "#" ;
			if (i==CurrentTest){
				strTemp+= "<-<-<-<-<-" ;
			}           
			else {
				strTemp+= "----------" ;
			}
			strTemp+= "#" ;
			strTemp+= Status_Num_to_String(Tests[i]->Status);
			strTemp+="#";
			strTemp+=TERM_CARRIAGE_RETURN;
			strTemp+= TERM_NEW_LINE ; 
			strTemp+="          #          #             #";
			strTemp+= TERM_CARRIAGE_RETURN ;
			strTemp+=TERM_NEW_LINE;
			//Serial.write(TERM_NEW_LINE);

		}
		strTemp+= "==========#==========#=============#";
		strTemp+= TERM_CARRIAGE_RETURN ;
		strTemp+=TERM_NEW_LINE;
		strTemp+="Use the keys 'u' and 'd' to move the cursor up and down.";
		strTemp+= TERM_CARRIAGE_RETURN ;
		strTemp+=TERM_NEW_LINE;
		strTemp+="Use ENTER to execute the sected option.";
		strTemp+= TERM_CARRIAGE_RETURN ;
		strTemp+=TERM_NEW_LINE;
		strTemp+="Use 'b' to go back to the previous menu.";
		//int j  ; // this variable is used to clear the rest of the screen, there is probably a better way to do this.
		//for (j=(30-i) ; j>0 ; j--){
		//    strTemp+= "                                                     ";  
		// }
		//Clear the screen
		Screen::clearScreen();
		Screen::cursorHome();
		//erasebelow();
		Serial.println(strTemp);
		//cursorHome();

		//Done printing the main screen

	}

	void run(){
		//This function parses the input of the keybord 
		// u to move the cursor Up 
		// d to move the cursor down 
		// ENTER to execute the program pointed to by the cursor
		// up arrow to move the cursor up, Note: when the up arrow is pressed the keyboard sends and array of three integers {27,91,65}
		// down arrow to move the cursor down, Note: when the down arrow is pressed the keyboard sends and array of three integers {27,91,66}
		// ESC the keyboard sends vlaue 27
		int current_input =1;
		printMain();
		boolean exit = false ;

		while (!exit){
			//Serial.println ("in while in run");
			current_input = readinput();
			if (current_input == up){
				if (CurrentTest==0){
					CurrentTest=LastTest;
				}
				else{
					CurrentTest--;
				}
				printMain();
			}
			else if (current_input==down){
				if (CurrentTest==LastTest){
					CurrentTest=0;
				}
				else {
					CurrentTest++;
				}
				printMain();
			}
			else if (current_input==ENTER){
				Tests[CurrentTest]->runTest();
				printMain();
			}else if (current_input==back ||current_input == ESC){
				exit = true ;
			}


		}
	}
};
//=======================================================================================
//Test Functions go here
//Each test/or option performed should be encapsulated in a function. 
//After the function is performed it should return a status of the option, the available options are {PASS ,FAIL ,NOT_PERFORMED  , NOT_TEST } 
//each one of these can have a test manager nested in it


int test1(){
	Serial.println("I am test 1");
	static int Test = -1 ; 
	return (Test ++)%4 ;  
}

int test2(){
	Serial.println("I am test 2");
	static int Test = -1 ; 
	return ((Test ++)%4) ; 
}


int test3(){
	Serial.println("I am test 3");
	static int Test = -1 ; 
	return (Test ++)%4 ; 
}

TestsManager * NestedManager; // declare the nested  Manager  
int test_Nester(){
	Serial.println("I am nester 1");
	NestedManager->run(); // start the nested manager , the nested manager can either be initialised here or in void setup();
}


int test_Nested_1(){
	Serial.println("I am nested 1");
	static int Test = -1 ; 
	return (Test ++)%4 ; 

}

int test_Nested_2(){
	Serial.println("I am nested 2");
	static int Test = -1 ; 
	return (Test ++)%4 ; 
}

int test_Nested_3(){
	Serial.println("I am nested 3");
	static int Test = -1 ; 
	return (Test ++)%4 ; 
}


//============================================================================
TestsManager * Manager; // declare the main Manager

void setup() {
	// Open serial communications and wait for port to open:
	Serial.begin(115200);
	while (!Serial) {
		; // wait for serial port to connect.
	}

	Manager = new TestsManager() ; //initialize the manager
	//add all the options you want to include in the manager 			   
	Manager->addTest("TEST_1    " , 'A' , 0 , &test1 , NOT_PERFORMED);
	Manager->addTest("TEST_2    " , 'B' , 1 , &test2 , NOT_PERFORMED);
	Manager->addTest("TEST_3    " , 'A' , 3 , &test1 , NOT_PERFORMED);
	Manager->addTest("TEST_4    " , 'C' , 2 , &test3 , NOT_PERFORMED);
	Manager->addTest("TEST_5    " , 'C' , 2 , &test3 , NOT_PERFORMED);
	Manager->addTest("TEST_6    " , 'C' , 2 , &test3 , NOT_PERFORMED);
	Manager->addTest("TEST_7    " , 'C' , 2 , &test3 , NOT_PERFORMED);
	Manager->addTest("TEST_8    " , 'C' , 2 , &test3 , NOT_PERFORMED);
	Manager->addTest("TEST_9    " , 'C' , 2 , &test3 , NOT_PERFORMED);
	Manager->addTest("Nester_1  " , 'C' , 2 , &test_Nester , NOT_PERFORMED);

	NestedManager = new TestsManager() ; //initialize the manager
	NestedManager->addTest("Nested_1  " , 'A' , 0 , &test_Nested_1 , NOT_PERFORMED);
	NestedManager->addTest("Nested_2  " , 'B' , 1 , &test_Nested_2 , NOT_PERFORMED);
	NestedManager->addTest("Nested_3  " , 'C' , 2 , &test_Nested_3 , NOT_PERFORMED);

}

//=============================================================================
void loop(){ 
	//Start the manager
	Manager->run();
}
//==============================================

