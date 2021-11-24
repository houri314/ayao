#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <conio.h>

#define ROOT_USERNAME "chien"
#define ROOT_PASSWORD "root"
#define TESTINGMODE
#define VERSION "0.0.5 TESTING"
#define SUCCESS 0
#define MEMORY_ERR 1
#define USER_ABORTED 2
#define HALT 3
#define NO_ANSI 4
#define OTHER 5

#define BIN_DIR "./root/os/bin/"
#define APP_DIR "./root/os/app/"
#define HOME_DIR "./root/home/"
#define ROOT "./root/"

#ifdef _WIN32
	#define ChienCLS system("cls")
	#define MainOS "Windows"
#endif
#ifdef _WIN64 
	#define ChienCLS system("cls")
	#define MainOS "Windows"
#endif
#ifdef __linux__
	#define ChienCLS system("clear")
	#define MainOS "Linux"
#endif
#ifdef TESTINGMODE
	#define sleep(x) sleep(0)
#endif

int _starting_kernel();
int _shutdown();
int _save_session(); //save session and shutdown, just like "Sleep" in Windows/Linux
int _continue_session(); //continue saved session. it should let the user know if the saved session was damaged
int _boot_halt();
int _sys_tree_created();
int _continue_loading_kernel();
int _create_system_folder();
int _login();
int _first_user();
int _register();
int _update_user();
int _kernel_main();
int _clear_all_args();
int _toDir(); //wtf
int _update_user(); //update user password
//int _command_to_arg(); //split command to args  //moved to _command_handler()

int _command_handler();
int _interrupt_handler();
int _get_command_from_buffer();

int _tip_of_the_day();
int _once();
int _ekko(); //done
int _chdir(); //*
int _password(); //*
int _logout(); //done
int _newuser(); //done
int _whoami(); //done
int _execute(); //*
int _sys_info(); //done
int _summonfile(); //create a  file in directory 
int _external_command(); //half done.
int _sys_info(); //done
int _add_app(); //*
int _pasukaru(); //this shit will be large as fuck. *
int _ced(); //this would also be big. ced = Chien EDit *
int _crea(); //read file and print to terminal. crea = Chien REAd. done Sep 29th 2021
int _help(); //remember to update this when update more commands *
 
char _SPARE_USER[255];
char _CURRENT_USER[255];
char _CURRENT_DIR[255];
char _COMMAND[255];
char _EXECUTING[255];
char _ARG[10][255];
char _COMMAND_BUFFER[255];
char _SYNTAX[255];
int _ISFIRSTBOOT;
int _ISSAVING;

char *strremove(char *str, const char *sub) {    //copy from stackoverflow lol
    char *p, *q, *r;
    if (*sub && (q = r = strstr(str, sub)) != NULL) {
        size_t len = strlen(sub);
        while ((r = strstr(p = r + len, sub)) != NULL) {
            while (p < r)
                *q++ = *p++;
        }
        while ((*q++ = *p++) != '\0')
            continue;
    }
    return str;
}

int _boot_display() {
	printf("ChienComputer, version %s\n", VERSION); sleep(1);
	printf("Last updated: %s %s\n",__DATE__,__TIME__); sleep(2);
	if (__STDC__==1) printf("Compiled with ANSI standard\n\n");
	else {
		ChienCLS;
		printf("Warning: this wasn't compiled with ANSI standard\n");
		printf("Halting boot process...\n\n\n");
		return _boot_halt();
	}
	sleep(2);
	printf("\tDeveloped by Ikora, Hourim, Hourin, randomInteger, Phuc Jaki\n");
	sleep(5);
	return _starting_kernel();
}

int _boot_mem_allocate() {	
	int *memtest = malloc(8000);
	if (memtest==NULL) {
		fprintf(stderr, "Unable to allocate required memory for booting ChienComputer");
		return MEMORY_ERR;
	} else {
		printf("Allocated 8KB. Booting ChienComputer");
	}
	sleep(3);
	ChienCLS;
	free(memtest);
	return SUCCESS;
}

int _sys_tree_created() {
	FILE *fp;
	fp = fopen("./root/os/bin/chiencomputer.dat", "r");
	if (fp==NULL) {
		printf("ChienComputer machine folder doesn't exist. "); return OTHER;
	} else {
		printf("Machine folder seems fine enough. "); return SUCCESS;
	}
}

int _boot();

int main() {
	_boot();
}

int _boot() {
	_boot_mem_allocate();
	_boot_display();
}

int _boot_halt() {
	if (__STDC__!=1) {
		printf("ChienComputer version %s cannot run properly if not compiled with ANSI standard or \
		the terminal doesn't support ANSI\n", VERSION);
		printf("Trying to run ChienComputer without ANSI will cause problems and isn't recommended.\n");
		printf("If you understood, press any keys to exit\n\n");
		getch();
		exit(NO_ANSI);
	}
}

int _starting_kernel() {
	if (_sys_tree_created()==OTHER) return _create_system_folder();
	else if (_sys_tree_created()==SUCCESS) return _continue_loading_kernel(); 
}

int _shutdown() {
	exit(SUCCESS);
}

int _create_system_folder() {
	printf(" Creating machine folder. Press any key to continue..."); sleep(4);
	getch();
	ChienCLS;
	printf("Creating root directory...\n"); sleep(2);
	mkdir("./root");
	printf("Creating OS directory...\n"); sleep(1);
	chdir("./root/");
	mkdir("./os");
	printf("Creating home directory...\n"); sleep(1);
	mkdir("./home");
	printf("Creating bin directory in OS...\n"); sleep(1);
	chdir("./os/");
	mkdir("./bin");
	printf("Creating global application folder...\n"); sleep(1);
	mkdir("./app");
	printf("Creating users folder...\n"); sleep(1);
	mkdir("./users");
	printf("Generating chiencomputer.dat file...\n"); sleep(2);
	chdir("./bin");
	FILE *chiencomputer;
	chiencomputer = fopen("chiencomputer.dat", "w");
	printf("Created system folders. Press any key to shutdown...\n");
	getch();
	return _shutdown();
}

int _login() {
	FILE *ifuserfile;
	/*char x[PATH_MAX];
	printf("%s",getcwd(x,sizeof(x)));*/
	ifuserfile = fopen("./root/os/users/chien.chien","r");
	if (ifuserfile==NULL) return _first_user();
	fclose(ifuserfile);
	
	char a[255];
	char p[255];
	char sparestr[255];
	char temp[255];
	while (1) {
		printf("\nUsername: ");
		gets(a);
		strcpy(sparestr,a);
		strcat(a,".chien");
		strcpy(temp, "./root/os/users/");
		strcat(temp,a);
		strcpy(a,temp);
		ifuserfile = fopen(a,"r");
		if (ifuserfile==NULL) {
			printf("User %s doesn't exist. Create user %s now? (y/n)\n",sparestr,sparestr);
			char y;
			while (1) {
				y = getch();
				if (y=='y') {
					strcpy(_SPARE_USER,sparestr);
					return _register();
				}
				else if (y=='n') return _login();
				else continue;
			}
		} else {
			fclose(ifuserfile);
			break;
		}       //add password input here later
	}
	FILE *loginuser;
	loginuser = fopen(a,"r");
	char correctpsw[255];
	char inputpsw[255];
	fgets(correctpsw,255,ifuserfile); //reads restriction
	fgets(correctpsw,255,loginuser); //reads password
	printf("Enter password for %s: ",sparestr);
	gets(inputpsw);
	if (strcmp(inputpsw,correctpsw)) {
		sleep(3);
		printf("Wrong password\n");
		return _continue_loading_kernel();
		return OTHER;
	}
	
	strcpy(_CURRENT_USER,sparestr);
	strcpy(_SPARE_USER,"");
	return _continue_loading_kernel();
}

int _register() {
	char str_username[255];
	char str_password[255];
	char str_path[255];
	if (!strcmp(_SPARE_USER,"")) {
		printf("\nCreate new user: \n");
		printf("Enter username (this can't be changed later): ");
		gets(str_username);
		strcpy(str_path,"./root/os/users/");
		strcat(str_path,str_username);
		strcat(str_path,".chien");
		FILE *checkexist; //check if user already exist
		checkexist = fopen(str_path,"r");
		if (checkexist!=NULL) {
			printf(">>Error: User %s already existed\n",str_username);
			return OTHER;
		}
	}
	else strcpy(str_username,_SPARE_USER);
	printf("Enter password for %s: ",str_username); // i will add string masking later (or never)
	gets(str_password);
	//checks if user already exists (done above)
	FILE *newuserfile;
	newuserfile = fopen(str_path,"w");
	fprintf(newuserfile, "RESTRICT_READONLY\n"); 
	fprintf(newuserfile, str_password);
	fclose(newuserfile);
	//return to kernel, logged in as user (done)
	char ch;
	printf("Login as %s now? (y/n)",str_username);
	while (1) {
		ch=getch();
		if (ch=='y') {
			strcpy(_CURRENT_USER,str_username);
			return _continue_loading_kernel();
		} else if (ch=='n') {
			return _continue_loading_kernel();
		} else continue;
	}
}

int _first_user() {
	char x; int satisfied=0;
	ChienCLS;
	printf("\nYou are now creating a root user for ChienComputer\n"); sleep(2);
	printf("Default root user username is %s\nDefault root user password is %s", ROOT_USERNAME, ROOT_PASSWORD);
	FILE *rootuser;
	rootuser = fopen("./root/os/users/chien.chien","w");
	fprintf(rootuser, "RESTRICT_READONLY\n");
	fprintf(rootuser, ROOT_PASSWORD);
	fclose(rootuser);
	printf("Do you want to create an another user or login as root?\n");
	printf("1. Login as root\n2. Create another user\n");
	while (satisfied!=1) {
		x=getch();
		if (x=='1') {
			strcpy(_CURRENT_USER,ROOT_USERNAME);
			return _continue_loading_kernel();
		}
		else if (x=='2') {
			return _register();
		} else ;
	}
}

int _kernel_main() {
	printf("\nCurrently logged in as %s\n\n\n",_CURRENT_USER);
	_once();
	while (1) {
		printf("%s:%s>",_CURRENT_USER,_CURRENT_DIR);
		gets(_COMMAND);
		_command_handler();
		_interrupt_handler();
		_clear_all_args();
	} 
}

int _clear_all_args() {
	int argIndex = 0;
	while (argIndex<=10) {
		strcpy(_ARG[argIndex],"");
		argIndex++;
	}
	return SUCCESS;
}

int _continue_loading_kernel() {
	if (!strcmp(_CURRENT_USER,"")) return _login();
	return _kernel_main();
}

int _tip_of_the_day() {
	int tipno = 0;
	if (tipno==0) return printf("\nTired of updates? Switch to Linux\n");
	else if (tipno==1) return printf("\nUse Linux for the best programming experience\n");
	else if (tipno==2) return printf("\nDo you know Terry A. Davis, the smartest programmer that's ever lived?\n");
	else if (tipno==3) return printf("\nPut exteral programs to ./root/os/app and run add-app to\
 put those apps into ./root/os/bin to use\n");
}

int _once() {
	strcpy(_SPARE_USER,"");
	printf("Random tip: ");
	_tip_of_the_day();
	if (_ISFIRSTBOOT!=1) {
	strcpy(_CURRENT_DIR, "./root/home/");
		printf("\nChienComputer is in testing stage. Some function might not be working as\n");
		printf("intended. Report bugs and potential exploits to ikora#6925\n");
	_ISFIRSTBOOT = 1;
	}
}

int _command_handler() {
	strcpy(_EXECUTING,_COMMAND);
	strcpy(_SYNTAX,_EXECUTING);
	//split arguments here
	char *token = strtok(_EXECUTING," ");
	int argIndex = 0;
	while (token!=NULL) {
		strcpy(_ARG[argIndex],token);
		//printf("%s\n",_ARG[argIndex]); testing args
		token=strtok(NULL," ");
		argIndex=argIndex+1;
	}
	//end splitting arguments
	strtok(_SYNTAX," ");
	strcpy(_COMMAND_BUFFER,_COMMAND);
	if (!strcmp(_SYNTAX,"chdir")) return _chdir();
	else if (!strcmp(_SYNTAX,"ekko")) return _ekko();
	else if (!strcmp(_SYNTAX,"password")) return _password();
	else if (!strcmp(_SYNTAX,"logout")) return _logout();
	else if (!strcmp(_SYNTAX,"newuser")) return _newuser();
	else if (!strcmp(_SYNTAX,"execute")) return _execute();
	else if (!strcmp(_SYNTAX,"sysinfo")) return _sys_info();
	else if (!strcmp(_SYNTAX,"whoami")) return _whoami();
	else if (!strcmp(_SYNTAX,"add-app")) return _add_app();
	else if (!strcmp(_SYNTAX,"shutdown")) return _shutdown();
	else if (!strcmp(_SYNTAX,"help")) return _help();
	else if (!strcmp(_SYNTAX,"pasukaru")) return _pasukaru();
	else if (!strcmp(_SYNTAX,"ced")) return _ced();
	else if (!strcmp(_SYNTAX,"crea")) return _crea();
	else if (!strcmp(_SYNTAX,"suf")) return _summonfile();
	else if (!strcmp(_SYNTAX,"")) return USER_ABORTED;
	else return _external_command();
}

int _interrupt_handler() { // check interrupts such as Ctrl+C or stuff
	
}

// i was working here. tomorrow i will return
int _ekko() {   //get directly from _COMMAND so i don't have to worry about _EXECUTING
	char texttosay[255];
	strcpy(texttosay,_COMMAND);
	if (!strcmp(texttosay,"ekko")) {
		printf("");
		return SUCCESS;
	} else if (!strcmp(texttosay,"ekko _nl")) {
		printf("\n");
		return SUCCESS;
	}
	strremove(texttosay,"ekko ");
	printf("%s\n",texttosay);
	return SUCCESS;
}

int _chdir() {
	//change directory
	if (!strcmp(_ARG[1],"")) return printf("%s\n",_CURRENT_DIR);
	else if (!strcmp(_ARG[1],".")) return SUCCESS;
	else if (!strcmp(_ARG[1],"..")) return printf("function not completed lmao\n");
	if (!strcmp(_ARG[1],"s>bin")) {
		strcpy(_CURRENT_DIR,BIN_DIR);
		return SUCCESS;
	} else if (!strcmp(_ARG[1],"s>app")) {
		strcpy(_CURRENT_DIR,APP_DIR);
		return SUCCESS;
	} else if (!strcmp(_ARG[1],"s>home")) {
		strcpy(_CURRENT_DIR,HOME_DIR);
		return SUCCESS;
	}
}

int _summonfile() {
	if (!strcmp(_ARG[1],"")) return printf(">>Expected 1 argument. No arguments provided\n");
	char fileatdir[255];
	strcpy(fileatdir,_CURRENT_DIR);
	strcat(fileatdir,_ARG[1]);
	FILE *uh;
	uh = fopen(fileatdir,"w");
	if (uh==NULL) {
		printf(">>Error: Couldn't create file %s\n",_ARG[1]);
		return OTHER;
	}
	return printf("Created file %s\n",_ARG[1]);
}

int _password() {
	//change password of current user
	printf("Change password of user %s? (y/n)\n",_CURRENT_USER);
	char ch;
	while (1) {
		ch=getch();
		if (ch=='y') return _update_user();
		else if (ch=='n') return USER_ABORTED;
		else continue; 
	}
}

int _logout() {
	//logout. switch user. the idea is i make a function to update current user
	//3 hours later update: turned out to be no need
	strcpy(_CURRENT_USER,""); sleep(2);
	printf("Logged out.\n"); 
	return _login();
}

int _update_user() {
	//printf("Enter %s's current password: \n",_CURRENT_USER);
	//check password here <-
	printf("Enter new password for %s: ", _CURRENT_USER);
	char newpassword[255];
	gets(newpassword); //have to struggle with replace line in text file later
	printf("function not completed lmao\n"); 
	return SUCCESS;
}

int _newuser() {
	char ya;
	printf("Do you want to create a new user? (y/n)\n");
	while (1) {
		ya=getch();
		if (ya=='y') return _register();
		else if (ya=='n') return USER_ABORTED;
		else ;
	}
}

int _execute() { //look in app because yes
	if (!strcmp(_ARG[0],"")) {
		printf(">>Expected 1 argument. No arguments provided.\n");
		return OTHER;
	} else if (!strcmp(_ARG[1],"")) {
		printf(">>Expected 1 argument. 2 or more arguments provided.\n");
		return OTHER;
	}
	if (!strcmp(MainOS,"Windows")) {
		system("");
	}
}

int _sys_info() {
	printf("\nLast updated %s %s\n",__DATE__,__TIME__); sleep(1);
	printf("Currently running ChienComputer Version %s\n",VERSION); sleep(1);
	printf("Developed and published by Ikora\n\n"); sleep(1);
	printf("");
	return SUCCESS;
}

int _external_command() { //after not found any standard commands, find external commands in /bin
	printf(">>Error: Command not found\n");
	return OTHER;
}

int _add_app() {
	if (!strcmp(_ARG[1],"")) {
		printf(">>Error: Expected 1 argument but no arguments provided\n");
		return OTHER;
	} else if (strcmp(_ARG[2],"")) {
		printf(">>Error: Expected 1 argument but 2 (or more) arguments provided\n");
		return OTHER;
	} else {
		//add app from .root/os/app to .root/os/bin
	}
}

int _whoami() {
	printf("You are currently logged in as %s\n",_CURRENT_USER);
	//root permission go here
	return SUCCESS;
}

int _pasukaru() { //going to be big
	//check error first
	if (!strcmp(_ARG[1],"")) {
		printf(">>Error: Expected 1 argument but no arguments provided\n");
		return OTHER;
	} else if (strcmp(_ARG[2],"")) {
		printf(">>Error: Expected 1 argument but 2 or more arguments provided\n");
		return OTHER;
	}
	//check special commands
	if (!strcmp(_ARG[1],"--help")) {
		printf("\nChienComputer Pasukaru Interpreter, developed by Ikora (whole language and interpreter)\n");
		printf("Usage: pasukaru [file directory]/[file name].pas\n");
		printf("Note: file directory must be full path from .\n");
		printf("   Example: pasukaru ./root/home/a.pas\n");
		return SUCCESS;
	}
	FILE *pascalfile;
	pascalfile = fopen(_ARG[1],"r");
	if (pascalfile==NULL) {
		printf("Cannot find file %s\n",_ARG[0]);
		return OTHER;
	} 
	char _command[255];
	char _executing[255];
	char _arg[10][255];
	char _syntax[255];
	//i still haven't thought of an idea about how does this work
	//read first lines in the pascal file
	//program name, uses crt, ect..
	fgets(_command,255,pascalfile); //program
	fgets(_command,255,pascalfile); //uses
	//my version of pascal will be different. it will use special stuff instead of variables
	fgets(_command,255,pascalfile); //BEGIN_PROC
	if (strcmp(_command,"BEGIN_PROC:")) {
		printf("<<Pascal Error: \"BEGIN_PROC:\" expected at line 3\n");
		return OTHER;
	}
	while (fgets(_command,255,pascalfile)) {
		//later
	}
	//main reading process above
	printf("Program exits with code 0\n");
	return SUCCESS;
}

int _help() {
	printf("\nList of Standard ChienComputer commands: \n");
	printf("help: display this list\n");
	printf("ekko [message]: display message (ekko _nl for newline)\n");
	printf("add-app [app name]: add app to ./root/os/bin/\n");
	printf("sysinfo: display system info\n");
	printf("logout: log out and end this user session\n");
	printf("newuser: create new user\n");
	printf("chdir [next directory]: change current working directory. Only 1 change at a time\n\n"); 
}

int _ced() {
	int shouldExit = 0;
	int mode = 0; //0: command mode, 1: edit mode 
	char filepath[255];
	if (!strcmp(_ARG[1],"")) {
		printf(">>Error: Expected 1 argument but no arguments provided\n");
		return OTHER;
	} else if (strcmp(_ARG[2],"")) {
		printf(">>Error: Expected 1 argument but 2 or more arguments provided\n");
		return OTHER;
	}
	if (!strcmp(_ARG[1],"--help")) {
		printf("\nChienComputer Text Editor, developed by Hourin and Hourim\n");
		printf("Usage: pascal [file directory]/[file name].[file extension]\n");
		printf("Note: file directory must be full path from '.'\n");
		printf("   Example: ced ./root/home/a.ctx\n");
		printf("To see list of commands, use ced --cmd\n");
		return SUCCESS;
	} else if (!strcmp(_ARG[1],"--cmd")) {
		printf("To enter edit mode, type m-edit\n");
		printf("To return to command mode, type >_m-command\n");
		printf("To put a line in edit mode, type that line and press Enter\n");
		printf("del-ll: delete last line\n"); //*
		printf("wq: exit ced\n");
		printf("edit-l [line number]: edit line number\n"); //*
		printf("rev: review file\n"); //not completed
		printf("del-l [line number]: delete line number\n"); //*
		return SUCCESS;
	}
	FILE *fp;
	strcpy(filepath,_CURRENT_DIR);
	strcat(filepath, _ARG[1]);
	//check permission
	char restriction[255];
	fp = fopen(filepath,"r");
	if (fopen==NULL) {
		printf(">>Error: Couldn't find file %s\n",_ARG[1]);
		return OTHER;
	}
	fgets(restriction,255,fp);
	if (!strcmp(restriction,"RESTRICT_READONLY")) {
		printf(">>Error: permission denied\n");
		return OTHER;
	} else if (!strcmp(restriction,"RESTRICT_ALL")) {
		printf(">>Error: permission denied\n");
		return OTHER;
	}
	fclose(fp);
	printf("function not completed lmao\n");
	char linetowrite[255];
	char command[255];
	printf("CED: Command mode\n");
	FILE *filetoedit;
	filetoedit=fopen(filepath,"w");
	while (shouldExit==0) {
		if (mode==0) {
			printf(">_ ");
			gets(command);
			//command handler
			if (!strcmp(command,"m-edit")) {
				printf("CED: Edit mode\n");
				mode=1;
			} else if (!strcmp(command,"wq")) {
				printf("CED: Returning to main OS\n");
				fclose(filetoedit);
				return SUCCESS;
			}
		}
		if (mode==1) {
			gets(linetowrite);
			if (!strcmp(linetowrite,">_m-command")||!strcmp(linetowrite,"> _m-command")) {
				printf("CED: Command mode\n");
				mode=0;
				continue;
			}
			strcat(linetowrite,"\n");
			fprintf(filetoedit,linetowrite);
		}
	}
	return SUCCESS;
}

int _crea() {
	if (!strcmp(_ARG[1],"")) {
		printf(">>Error: Expected 1 argument but no arguments provided\n");
		return OTHER;
	} else if (strcmp(_ARG[2],"")) {
		printf(">>Error: Expected 1 argument but 2 or more arguments provided\n");
		return OTHER;
	}
	if (!strcmp(_ARG[1],"--help")) {
		printf("\nChienComputer Text Viewer, developed by Phuc Jaki\n");
		printf("Usage: crea [file directory]/[file name].[file extension]\n");
		printf("Note: file directory must be full path from '.'\n");
		printf("   Example: crea ./root/home/a.ctx\n");
		return SUCCESS;
	}
	FILE *freakd;
	freakd = fopen(_ARG[1],"r");
	char tedir[255];
	strcpy(tedir,_CURRENT_DIR);
	strcat(tedir,_ARG[1]);
	if (freakd==NULL) {
		fclose(freakd);
		freakd = fopen(tedir,"r");
		if (freakd==NULL) {
			printf("Couldn't open file %s",_ARG[1]);
			return OTHER;
		}
	}
	char lineToRead[255];
	fgets(lineToRead,255,freakd);
	if (!strcmp(lineToRead,"RESTRICT_ALL")) {
		printf(">>Error: permission denied\n");
		return OTHER;
	}
	printf("%s",lineToRead);
	while (fgets(lineToRead,255,freakd)) {
		printf("%s",lineToRead);
	}
	printf("\n");
	return SUCCESS;
}
