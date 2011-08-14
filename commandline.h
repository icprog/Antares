#pragma once

#include "antares.h"

namespace Antares {


	using namespace System;
	using namespace System::Text;

	public ref class TransferOptions
	{
	public:
		String^ pattern;           // Wildcard pattern for files. Or empty string for all.
		array<String^>^ exclude_patterns; 

		CopyMode copymode;

		void init(void)
		{
			this->pattern="";
			this->copymode=CopyMode::UNDEFINED;
			this->exclude_patterns = gcnew array<String^>(0);

		}


		TransferOptions(void)
		{
			this->init();

		}

		TransferOptions(CopyMode mode)
		{
            this->init();
			copymode= mode;
		}



	};

	public ref class CommandLine
	{
	public:


		String^ str; // Input string
		array<String^>^ tokens;

		String^ the_command;
		bool command_dispatched;

		String^ cmd_param1;
		String^ cmd_param2;
		bool e;
		bool showgui;
		bool showgui_specified;
		bool exit_on_completion;
		bool no_prompt;
		bool turbo_specified;
		int turbo_mode;
		bool recurse;
		array<String^>^ exclude_patterns;

		int pid;


		void error(String ^ str)
		{
			Console::WriteLine(str);
			this->e = true;

		}

		void set_defaults(void)
		{
			this->e = false;
			this->command_dispatched = false;
			this->the_command="";
			this->cmd_param1="";
			this->cmd_param2="";
			this->showgui=true;
			this->showgui_specified=false;
			this->no_prompt = false;
			this->exit_on_completion=false;
			this->turbo_specified=false;
			this->recurse=false;
			this->exclude_patterns = gcnew array<String^>(0);
			this->exit_on_completion = true;
			this->pid=-1;



		}
		void analyse(void)
		{

			int nt = this->tokens->Length;
			int ind=1;
			while(ind<nt)
			{
				String^ tok = this->tokens[ind];

				if (tok=="cp" || tok=="mv") tok="-"+tok;

				if (tok=="-cp" || tok=="-mv" || tok=="/cp" || tok=="/mv")

				{
					String^ cmd = tok->Substring(1,2);
					if (the_command->Length>0)
					{
						this->error("ERROR: The commands "+the_command+" and "+cmd+" can't be used at the same time.");
						goto out;
					}
					this->the_command = cmd;


					if (ind + 2 >= nt)
					{
						this->error("ERROR: The command "+the_command+" requires both a source and destination.");
						goto out;
					}
					ind++;
					this->cmd_param1 = this->tokens[ind];
					ind++;
					this->cmd_param2 = this->tokens[ind];
					ind++;
					continue;
				}

				if (tok=="-g" || tok=="/g")
				{
					this->showgui_specified=true;
					this->exit_on_completion=false;
					ind++;
					continue;
				}
				if (tok=="-t" || tok=="/t" || tok == "\t1" || tok=="-t1")
				{
					this->turbo_specified=true;
					this->turbo_mode=1;
					ind++;
					continue;
				}
				if (tok == "-t0" || tok=="/t0")
				{
					this->turbo_specified=true;
					this->turbo_mode=0;
					ind++;
					continue;
				}

				if (tok == "-r" || tok =="/r")
				{
					this->recurse=true;
					ind++;
					continue;
				}

				if (tok == "-x" || tok == "/x")
				{
					if (ind+1 >= nt)
					{
						this->error("ERROR: The option "+tok+" requires one argument.");
						goto out;
					}
					ind++;
					String ^x = this->tokens[ind];
					array<wchar_t>^ sep = {',',';'};
			        array<String^>^ z = x->Split(sep,StringSplitOptions::RemoveEmptyEntries);
					int L1 = this->exclude_patterns->Length;
					int L2 = z->Length;
					Array::Resize(this->exclude_patterns,L1+L2);
					z->CopyTo(this->exclude_patterns,L1);
					ind++;
					continue;
				}

				if (tok == "-?" || tok == "--?" || tok == "--help" || tok == "/?" || tok=="/help" )
				{
					Console::WriteLine("\nYou selected the "+tok+" option. ");
					Console::WriteLine("For help with the command line, please see the Antares home page.");
					Console::WriteLine("http://users.on.net/~henry/antares/commandline.html\n");
					ind++;
					continue;
				}

				if (tok == "-pid" || tok == "/pid")
				{
					if (ind+1 >= nt)
					{
						this->error("ERROR: The option "+tok+" requires one argument.");
						goto out;
					}
					ind++;
					String^ x = this->tokens[ind];
					bool invalid=false;
					try{
						this->pid = Convert::ToInt32(x,16);
					}
					catch(...)
					{
						invalid=true;
					}
					if (invalid || this->pid<=0 || this->pid>=65535)
					{
						this->error("ERROR: The option "+tok+" "+x+" did not specify a valid USB pid.");
						goto out;
					}




					ind++;
					continue;

				}




				this->error("ERROR: Unknown command line option: "+tok);
				goto out;
			}

out:

			if (this->the_command->Length>0)
			{
				this->showgui= this->showgui_specified;
				this->no_prompt = true;
				this->exit_on_completion = !this->showgui;
			}




		}


		void parse(void)
		{
			StringBuilder^ sb = gcnew StringBuilder(str);
			int L = str->Length;
			bool escaped = false;
			int i2=0;
			for (int i=0; i<L; i++)
			{
				if (!escaped)
				{
					if (str[i]==' ')
					{
						sb[i2]='\n';
						i2++;
						continue;
					}
					if (str[i]=='"')
					{
						escaped = true;
						continue;

					}
				}

				if (escaped && sb[i]=='"') 
				{
					escaped=false;
					continue;
				}
				sb[i2]=str[i];
				i2++;

			}
			sb->Length = i2;
			String ^str2 = sb->ToString();
			array<wchar_t>^ sep = {'\n'};
			tokens = str2->Split(sep,StringSplitOptions::RemoveEmptyEntries)  ;

		}



		CommandLine(String ^ inp_str)
		{

			this->str = inp_str;
			this->parse();
			this->set_defaults();
			this->analyse();

			//for each (String^ x in tokens)
			//	printf("Tok: $%s$\n",x);


		}

	};


}
