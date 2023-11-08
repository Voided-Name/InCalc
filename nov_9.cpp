#include <conio.h>
#include <direct.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iterator>
#include <limits>
#include <string>
#include <vector>
#include <map>
#include <cmath>
#include <algorithm>

using namespace std;

namespace fs = filesystem;
map<string, string> variables;

// misc
void clear_screen()
{
  // system("CLS");
}
void enter_to_continue()
{
  int ch;
  while (true)
  {
    cout << endl
         << "Press Enter to Continue...";

    ch = getch();

    if (ch == 13)
    {
      break;
    }
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    clear_screen();
    cout << endl;
  }
}
string remove_trailing_zeros(string outpt)
{
  int first_trailing, decimal_pos;
  bool found;
  for (int x = 0; x < outpt.length(); x++)
  {
    if (outpt[x] == '.')
    {
      decimal_pos = x;
    }
  }
  for (int x = decimal_pos + 1; x < outpt.length(); x++)
  {
    if (outpt[x] != '0')
    {
      first_trailing = 0;
      found == false;
    }
    else if (found == false)
    {
      if (outpt[x] == '0')
      {
        first_trailing = x;
        found = true;
      }
    }
  }

  if (found == true)
  {
    outpt.erase(first_trailing, string::npos);
  }
  return outpt;
}

// encryption handling
string encrypt(string password)
{
  string en_password;
  const string KEY = "CICT";
  int y = 0, key_last = KEY.length() - 1;
  int en_char;

  for (int x = 0; x < password.length(); x++)
  {
    int pass_x = password[x];
    int key_y = KEY[y];
    if (pass_x == ' ')
    {
      en_password += ' ';
      y++;
      continue;
    }
    en_char = pass_x + key_y;

    if (en_char > 125)
    {
      en_char = en_char - 125 + 32;
    }

    if (en_char == 59)
    {
      en_char = 126;
    }

    en_password += en_char;
    if (y == key_last)
    {
      y = 0;
    }
    else
    {
      y++;
    }
  }

  return en_password;
}
string decrypt(string password)
{
  string dec_password;
  const string KEY = "CICT";
  int y = 0, key_last = KEY.length() - 1;
  int en_char;

  for (int x = 0; x < password.length(); x++)
  {
    en_char = password[x];
    if (en_char == ' ')
    {
      dec_password += ' ';
      y++;
      continue;
    }

    if (en_char == 126)
    {
      en_char = 59;
    }
    if (en_char < 33 + KEY[y])
    {
      en_char = en_char - 32 + 125 - KEY[y];
    }

    dec_password += en_char;

    if (y == key_last)
    {
      y = 0;
    }
    else
    {
      y++;
    }
  }

  return dec_password;
}
string password_reg()
{
  string password;
  bool upper_true = false, number_true = false;
  int ch;

  while (true)
  {
    password = "";
    cout << "Password (at least one uppercase letter and one number, don't use "
            "whitespaces): ";
    ch = getch();

    while (ch != 13)
    {
      if (ch == 3)
      {
        abort();
      }
      if (ch == 8)
      {
        if (password.length() > 0)
        {
          cout << "\b \b";
          password.pop_back();
        }
        ch = getch();
        continue;
      }
      password.push_back(ch);
      cout << '*';
      ch = getch();
    }

    for (int x = 0; x < password.length(); x++)
    {
      if (password[x] >= 65 && password[x] <= 90)
      {
        upper_true = true;
      }
      if (password[x] >= 48 && password[x] <= 57)
      {
        number_true = true;
      }
    }

    if (upper_true && number_true)
    {
      break;
    }
    cin.clear();
    cout << endl
         << "Invalid Input! Please Try Again (no uppercase letter and/or no number)" << endl;
  }

  cout << endl;

  return password;
}
string get_password(string name)
{
  name = encrypt(name);
  vector<string> unames, passwords;
  string uname, pw, buffer;
  string path_pw = "usr/pw.txt";
  ifstream pw_get(path_pw);

  while (getline(pw_get, uname, ';'))
  {
    if (uname == name)
    {
      getline(pw_get, pw);
      break;
    }
    else
    {
      getline(pw_get, buffer);
    }
  }

  pw_get.close();
  return pw;
}
// file management
vector<string> users_sort(vector<string> filenames)
{
  vector<string> users;
  vector<string> users_vec_buffer;
  string users_buffer, prev_buffer;

  for (int x = 0; x < filenames.size(); x++)
  {
    users_buffer = "";
    for (int y = 0; y < filenames[x].length(); y++)
    {
      if (filenames[x][y] == ' ' || filenames[x][y] == '\\' ||
          filenames[x][y] == 0)
      {
        break;
      }
      users_buffer += filenames[x][y];
    }
    if (string(users_buffer) != string(prev_buffer))
    {
      users.push_back(users_buffer);
    }
    prev_buffer = users_buffer;
  }

  int y = 0;

  return users;
}
vector<string> scan_users()
{
  vector<string> filenames;
  fs::path p = fs::current_path() / "files";
  for (const auto &entry : fs::directory_iterator(p))
  {
    filenames.push_back(entry.path().string().substr(p.string().length() + 1));
  }

  return filenames;
}
vector<string> scan_files(string username)
{
  fs::path p = fs::current_path();
  vector<string> filenames;

  p = p / "files/" / username;

  for (const auto &entry : fs::directory_iterator(p))
  {
    filenames.push_back(entry.path().string().substr(p.string().length() + 1));
  }

  return filenames;
}
vector<vector<string>> get_dir_files_names(vector<string> filenames)
{
  string buffer, loop_read;
  int dir_index = 0;
  vector<vector<string>> dir_files;
  vector<string> dir, file, to_check;

  for (int x = 0; x < filenames.size(); x++)
  {
    for (int y = 0; y < filenames[x].length(); y++)
    {
      buffer += filenames[x][y];
    }
    if (buffer.length() <= 4)
    {
      if (empty(dir))
      {
        dir.push_back(buffer);
      }
      else if (!(buffer == dir[dir_index]))
      {
        dir.push_back(buffer);
        dir_index++;
      }
    }
    else if (buffer.substr(buffer.length() - 4, 4) == ".txt")
    {
      file.push_back(buffer);
    }
    else
    {
      if (empty(dir))
      {
        dir.push_back(buffer);
      }
      else if (!(buffer == dir[dir_index]))
      {
        dir.push_back(buffer);
        dir_index++;
      }
    }
    buffer = "";
  }

  dir_files.push_back(dir);
  dir_files.push_back(file);
  return dir_files;
}
// menu
string eval(string inpt);
vector<string> get_equations(vector<string> stream)
{
  int index_del = 0;
  if (stream[0] == "---")
  {
    for (int x = 1; x < stream.size(); x++)
    {
      index_del++;
      if (stream[x] == "---")
      {
        break;
      }
    }
    stream.erase(stream.begin() + 0, stream.begin() + index_del + 1);
  }

  stream.insert(stream.begin(), "---");
  for (const auto &x : variables)
  {
    if (x.first == "ans")
    {
      continue;
    }
    stream.insert(stream.begin(), x.first + "=" + x.second);
  }
  stream.insert(stream.begin(), "---");

  return stream;
}
void variables_analysis(vector<string> stream)
{
  bool split = false;
  string first_val, second_val;
  if (stream[0] == "---")
  {
    for (int x = 1; x < stream.size(); x++)
    {
      if (stream[x] == "---")
      {
        break;
      }
      else
      {
        for (int y = 0; y < stream[x].length(); y++)
        {
          if (stream[x][y] == '=')
          {
            split = true;
          }
          else if (!split)
          {
            first_val = first_val + stream[x][y];
          }
          else if (split)
          {
            second_val = second_val + stream[x][y];
          }
        }
        variables.insert({first_val, second_val});
        first_val = "";
        second_val = "";
        split = false;
      }
    }
  }
}
void open_file(string username, string file_topen)
{
  fs::path p = fs::current_path();
  p = p / "files/" / username;
  string inpt, oupt, inpt_stream, oupt_stream;
  vector<string> stream;

  ifstream input(p / file_topen);

  if (input.is_open())
  {
    string line;

    while (getline(input, line))
    {
      // Write each line to the output file
      cout << line << "\n";
      stream.push_back(line);
    }
  }
  input.close();

  if (!empty(stream))
  {
    variables_analysis(stream);
  }

  cout << "\n";
  cout << "<InCalc>\n\n";
  while (true)
  {
    cout << ">>> ";
    getline(cin, inpt_stream);
    if (inpt_stream == "quit()")
    {
      break;
    }
    else if (inpt_stream == "help()")
    {
      cout << "=====InCalc=====\n"
           << "quit() - quit application\n"
           << "vars() - show available variables\n"
           << "// - add a comment, Example:\n"
           << "\t>>> //This is a comment\n"
           << "\tcomment added\n"
           << "=====InCalc follows the following order=====\n -> FPMDAS\n- functions/variables\n- parentheses\n- multiplication\n- division\n- addition\n- subtraction\n"
           << "=====InCalc Calculations Example=====\n"
           << ">>> 3 + 4 * pow(3, 4)\n327\n"
           << "Creating a Variable: \n"
           << ">>> x = y + 2\n"
           << "variable added\n"
           << ">>> y = 5\n"
           << "variable added\n"
           << ">>> x + 4\n"
           << "11\n"
           << ">>> y = 6\n"
           << "changed variable\n"
           << ">>> x + 4\n"
           << "12\n"
           << "InCalc variables are similar to math equations, the variable will store its formula and when a variable inside another variable is changed it will change its own value\n\n"
           << "=====Available Functions=====\n"
           << "\t>>> pow(3, 2)\n"
           << "\t9\n"
           << "\t>>> sqrt(4)\n"
           << "\t2\n"
           << "\t>>> abs(-8)\n"
           << "\t8\n"
           << "\t>>> cbrt(-4)\n"
           << "\t-1.587401\n"
           << "\t>>> cos(0)\n"
           << "\t1\n"
           << "\t>>> sin(90)\n"
           << "\t0.893997\n"
           << "\t>>> tan(80)\n"
           << "\t9.003655\n"
           << "\t>>> floor(2.5)\n"
           << "\t2\n"
           << "\t>>> ceil(2.7)\n"
           << "\t3\n"
           << "\t>>> round(89.9)\n"
           << "\t90\n"
           << "\t>>> trunc(45.3)\n"
           << "\t45\n";
    }
    else if (inpt_stream.substr(0, 2) == "//")
    {
      stream.push_back("[" + inpt_stream.substr(2) + "]");
      cout << "\ncomment added\n";
    }
    else if (inpt_stream == "vars()")
    {
      cout << "\n-- Variables --\n";
      for (const auto &x : variables)
      {
        cout << x.first << " = " << x.second << "\n";
      }
      cout << "-- Variables --\n\n";
    }
    else
    {
      stream.push_back(">>> " + inpt_stream);
      oupt_stream = eval(inpt_stream);

      if (isdigit(oupt_stream[0]))
      {
        if (variables.find("ans") != variables.end())
        {
          variables["ans"] = oupt_stream;
        }
        else
        {
          variables.insert({"ans", oupt_stream});
        }
      }
      if (oupt_stream.find('.') != string::npos)
      {
        oupt_stream = remove_trailing_zeros(oupt_stream);
        cout << oupt_stream << endl;
        stream.push_back(oupt_stream);
      }
      else
      {
        cout << oupt_stream << endl;
        stream.push_back(oupt_stream);
      }
    }
  }

  if (empty(stream))
  {
    return;
  }
  stream = get_equations(stream);

  ofstream output_2(p / file_topen);
  for (int x = 0; x < stream.size(); x++)
  {
    output_2 << stream[x] << endl;
  }

  output_2.close();
}
string menu_loop_reader()
{
  int c;

  c = _getch();
  switch (c)
  {
  case 224:
    c = _getch();
    switch (c)
    {
    case 72:
      return "UP";
    case 80:
      return "DOWN";
    }
    break;
  case 13:
    return "ENTER";
  case 3:
    abort();
  case 97:
    return "add";
  case 100:
    return "delete";
  case 113:
    return "quit";
  case 109:
    return "mkdir";
  default:
    return "please_warning";
  }
  return "INVALID";
}
void menu_file_sel(string username)
{
  vector<string> filenames;

  filenames = scan_files(username);

  vector<vector<string>> dir_files = get_dir_files_names(filenames);
  vector<string> dir = dir_files[0];
  vector<string> file = dir_files[1];
  int loop_reader_num = 0;
  string loop_read, add_file, marked, mkdir_name;
  char y_n;

  while (true)
  {
    int y = 0;

    cout << "/" << username << endl;
    if (empty(dir) && empty(file))
    {
      cout << "No Directory or Files (press 'a' to add a file or 'm' to create a directory)" << endl;
    }
    for (int x = 0; x < dir.size(); x++)
    {
      if (loop_reader_num == y)
      {
        cout << ">";
        marked = dir[x];
      }
      else
      {
        cout << " ";
      }
      cout << "/" << dir[x] << endl;
      y++;
    }

    for (int x = 0; x < file.size(); x++)
    {
      if (loop_reader_num == y)
      {
        cout << ">";
        marked = file[x];
      }
      else
      {
        cout << " ";
      }
      cout << file[x] << endl;
      y++;
    }
    cout << "\n - UP and DOWN arrow keys to navigate\n - ENTER -> enter file or directory\n - 'a' -> add file \n - 'd' -> delete file\n - 'm' -> make directory\n - 'q' -> quit or escape\n";
    loop_read = menu_loop_reader();
    if (loop_read == "UP")
    {
      loop_reader_num--;
    }
    else if (loop_read == "DOWN")
    {
      loop_reader_num++;
    }
    else if (loop_read == "ENTER")
    {
      clear_screen();
      if (loop_reader_num < dir.size())
      {
        menu_file_sel(username + "/" + dir[loop_reader_num]);
      }
      else
      {
        open_file(username, file[loop_reader_num - dir.size()]);
      }
    }
    else if (loop_read == "add")
    {
      cout << endl
           << "Enter File Name: ";
      getline(cin, add_file);

      if (fs::exists("files/" + username + "/" + add_file + ".txt"))
      {
        cout << endl
             << "File Already Exists";
        enter_to_continue();
      }
      else
      {
        ofstream addfile("files/" + username + "/" + add_file + ".txt");
        addfile.close();
        cout << endl
             << "Created " + add_file;
        enter_to_continue();
      }

      filenames = scan_files(username);

      dir_files = get_dir_files_names(filenames);
      dir = dir_files[0];
      file = dir_files[1];
    }
    else if (loop_read == "mkdir")
    {
      cout << endl
           << "Enter Directory Name: ";
      getline(cin, mkdir_name);

      if (fs::exists("files/" + username + "/" + mkdir_name))
      {
        cout << endl
             << "Directory Already Exists";
        enter_to_continue();
      }
      else
      {
        fs::create_directory("files/" + username + "/" + mkdir_name);
        cout << endl
             << "Created " + mkdir_name;
        enter_to_continue();
      }

      filenames = scan_files(username);

      dir_files = get_dir_files_names(filenames);
      dir = dir_files[0];
      file = dir_files[1];
    }
    else if (loop_read == "delete")
    {
      y_n = 'a';
      while (y_n != 'y' && y_n != 'n')
      {
        cout << "Are you sure you want to delete(y/n) " << marked << ": ";
        cin >> y_n;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
      }
      if (y_n == 'y')
      {
        fs::remove_all("files/" + username + "/" + marked);
        filenames = scan_files(username);

        dir_files = get_dir_files_names(filenames);
        dir = dir_files[0];
        file = dir_files[1];
      }
    }
    else if (loop_read == "quit")
    {
      return;
    }
    if (loop_reader_num == dir.size() + file.size())
    {
      loop_reader_num = 0;
    }
    if (loop_reader_num < 0)
    {
      loop_reader_num = dir.size() + file.size() - 1;
    }
    clear_screen();
  }
}
void loginform()
{
  vector<string> users;
  users = scan_users();
  string name, password;
  bool username_exists;

  do
  {
    username_exists = false;
    cout << "Input Username: ";
    getline(cin, name);

    if (name == "quit()")
    {
      return;
    }

    password = password_reg();

    for (int x = 0; x < users.size(); x++)
    {
      if (name == users[x])
      {
        username_exists = true;
      }
    }

    if (username_exists == false)
    {
      cout << "Invalid Username or Password";
      enter_to_continue();
      clear_screen();
    }
  } while (!username_exists);

  if (encrypt(password) != get_password(name))
  {
    cout << "Invalid Username or Password";
  }
  else
  {
    cout << "Logged In";
    enter_to_continue();
    clear_screen();
    menu_file_sel(name);
  }
  enter_to_continue();
}
void regform()
{
  vector<string> users;
  users = scan_users();
  string name, password, password_2;
  bool has_space = false, username_exists;

  do
  {
    has_space = false;
    username_exists = false;

    cout << "Input Username: ";
    getline(cin, name);

    clear_screen();

    for (int x = 0; x < name.length(); x++)
    {
      if (name[x] == ' ')
      {
        cout << "Don't Include Whitespaces!" << endl;
        has_space = true;
      }
    }
    for (int x = 0; x < users.size(); x++)
    {
      if (name == users[x])
      {
        username_exists = true;
        cout << "Username is already used! Please use a different username..."
             << endl
             << "type quit() to exit if you do not wish to proceed yet" << endl;
      }
    }
    if (name == "quit()")
    {
      return;
    }
  } while (has_space || username_exists);
  password = password_reg();
  cout << "Confirm ";
  password_2 = password_reg();

  if (password != password_2)
  {
    cout << "Password does not match!\n";
    enter_to_continue();
    return;
  }

  cout << "\nRegistered " << name << "\n";

  ofstream pw_file("usr/pw.txt", ofstream::app);
  pw_file << encrypt(name) << ";" << encrypt(password) << endl;
  name = "files/" + name;
  fs::create_directory(name);
  pw_file.close();

  enter_to_continue();
}
void delform()
{
  vector<string> users, del_vec;
  users = scan_users();
  string name, password, del_str, del_str_2;
  bool username_exists = false;
  char choice = 'c';

  cout << "Input Username: ";
  getline(cin, name);

  password = password_reg();

  for (int x = 0; x < users.size(); x++)
  {
    if (name == users[x])
    {
      username_exists = true;
    }
  }

  if (username_exists)
  {
    if (encrypt(password) == get_password(name))
    {
      while (choice != 'y' && choice != 'n')
      {
        cout << "\n Are you want to delete your account?(y/n): ";
        cin >> choice;
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
      }
    }
  }

  if (choice == 'y')
  {
    ifstream pw_file("usr/pw.txt");
    while (getline(pw_file, del_str, ';'))
    {
      if (encrypt(name) == del_str)
      {
        getline(pw_file, del_str);
      }
      else
      {
        getline(pw_file, del_str_2);
        del_str = del_str + ";" + del_str_2;
        del_vec.push_back(del_str);
      }
    }

    ofstream out_del("usr/pw.txt");
    for (const auto &x : del_vec)
    {
      out_del << x << "\n";
    }
    fs::remove_all("files/" + name);
    cout << "Account Deleted\n";
    enter_to_continue();
  }
}
void menu_loop()
{
  string log_sign_str[3] = {"Login", "Sign Up", "Delete Account"};
  string arrow_inpt;

  int pos = 0;
  while (true)
  {
    for (int x = 0; x < 3; x++)
    {
      if (x == pos)
      {
        cout << ">";
      }
      else
      {
        cout << " ";
      }
      cout << log_sign_str[x] << std::endl;
    }
    cout << "(Use the Up and Down arrow keys to select and the Enter key to "
            "proceed | press q to quit)"
         << endl;

    arrow_inpt = menu_loop_reader();

    if (arrow_inpt == "UP")
    {
      pos--;
      if (pos < 0)
      {
        pos = 0;
      }
    }
    else if (arrow_inpt == "DOWN")
    {
      pos++;
      if (pos > 2)
      {
        pos = 2;
      }
    }
    else if (arrow_inpt == "ENTER")
    {
      if (pos == 0)
      {
        clear_screen();
        loginform();
      }
      else if (pos == 1)
      {
        clear_screen();
        regform();
      }
      else
      {
        delform();
      }
    }
    else if (arrow_inpt == "quit")
    {
      break;
    }

    clear_screen();
  }
}
// main program logic
bool is_classic_operator(char cha)
{
  if (cha == '+' || cha == '/' || cha == '*' || cha == '-')
  {
    return true;
  }
  return false;
}
bool is_valid_operator(string op)
{
  string valid_op[12] = {"+", "-", "*", "/", "++", "+-",
                         "--", "-+", "*-", "*+", "/+", " /-"};
  for (int x = 0; x < 12; x++)
  {
    if (op == valid_op[x])
    {
      return true;
    }
  }
  return false;
}
string remove_spaces(string inpt)
{
  string buffer;

  for (const auto &x : inpt)
  {
    if (!isspace(x))
    {
      buffer += x;
    }
  }
  return buffer;
}
bool is_a_z(char inpt)
{
  return (isalpha(inpt));
}

string is_buffer_valid_func(string inpt)
{
  bool valid = false;
  string test[] = {"pow", "sqrt", "abs", "cbrt", "cos", "sin", "tan", "floor", "ceil", "round", "trunc", "mean", "median", "mode"};
  for (const auto &x : test)
  {
    if (inpt == x)
    {
      return inpt;
      valid = true;
    }
  }
  return "invalid";
}

struct fn_return
{
  double answer;
  int index;
  string error;
  string special;
  string special_val;
};
fn_return func_eval(string buffer, int x, string inpt, string valid_func)
{
  int par_left = 1;
  int par_right = 0;
  int index = 0;
  fn_return fn_ret;
  vector<string> answer_buffer;
  double answer;
  bool error002 = false;

  fn_ret.error = "";

  answer_buffer.push_back("");
  while (par_left != par_right)
  {
    if (x > inpt.length())
    {
      cout << "Invalid format of Function\n";
      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR007 - Invalid Format of Function";
      return fn_ret;
    }
    if (inpt[x] == '(')
    {
      answer_buffer[index] += inpt[x];
      par_left++;
    }
    else if (inpt[x] == ')')
    {
      par_right++;
      if (par_left == par_right)
      {
        break;
      }
      answer_buffer[index] += inpt[x];
    }
    else if (inpt[x] == ',')
    {
      if (par_left > 1 && par_right == par_left - 1)
      {
        index++;
        answer_buffer.push_back("");
      }
      else if (par_left == 1)
      {
        index++;
        answer_buffer.push_back("");
      }
      else
      {
        answer_buffer[index] += inpt[x];
      }
    }
    else
    {
      answer_buffer[index] += inpt[x];
    }
    x++;
  }

  for (int x = 0; x < answer_buffer.size(); x++)
  {
    answer_buffer[x] = eval(answer_buffer[x]);
  }
  if (valid_func == "pow")
  {
    if (answer_buffer.size() != 2)
    {
      cout << "Invalid Number of Inputs for pow, must be two inputs, Ex: pow(3, 2)\n";
      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR002";
      return fn_ret;
    }

    answer = pow(stod(answer_buffer[0]), stod(answer_buffer[1]));
  }
  else if (valid_func == "sqrt")
  {
    if (answer_buffer.size() != 1)
    {
      cout << "Invalid number of inputs for sqrt, must be one argument, Ex: sqrt(2)\n";

      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR002";

      return fn_ret;
    }
    if (stod(answer_buffer[0]) < 0)
    {
      cout << "Invalid input for sqrt, sqrt is unable to process negative numbers\n";

      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR005";

      return fn_ret;
    }

    answer = sqrt(stod(answer_buffer[0]));
  }
  else if (valid_func == "abs")
  {
    if (answer_buffer.size() != 1)
    {
      cout << "Invalid number of inputs for abs, must be one argument, Ex: abs(6)\n";
      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR002";
      return fn_ret;
    }
    if (stod(answer_buffer[0]) < 0)
    {
      answer_buffer[0].erase(0, 1);
      answer = stod(answer_buffer[0]);
    }
    else
    {
      answer = stod(answer_buffer[0]);
    }
  }
  else if (valid_func == "cbrt")
  {
    if (answer_buffer.size() != 1)
    {
      cout << "Invalid number of inputs for cbrt, must be one argument, Ex: cbrt(2)\n";
      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR002";
      return fn_ret;
    }
    answer = cbrt(stod(answer_buffer[0]));
  }
  else if (valid_func == "cos")
  {
    if (answer_buffer.size() != 1)
    {
      cout << "Invalid number of inputs for cos, must be one argument, Ex: cos(2)\n";
      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR002";
      return fn_ret;
    }
    answer = cos(stod(answer_buffer[0]));
  }
  else if (valid_func == "sin")
  {
    if (answer_buffer.size() != 1)
    {
      cout << "Invalid number of inputs for sin, must be one argument, Ex: sin(2)\n";
      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR002";
      return fn_ret;
    }
    answer = sin(stod(answer_buffer[0]));
  }
  else if (valid_func == "tan")
  {
    if (answer_buffer.size() != 1)
    {
      cout << "Invalid number of inputs for tan, must be one argument, Ex: tan(2)\n";
      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR002";
      return fn_ret;
    }
    answer = tan(stod(answer_buffer[0]));
  }
  else if (valid_func == "floor")
  {
    if (answer_buffer.size() != 1)
    {
      cout << "Invalid number of inputs for floor, must be one argument, Ex: floor(2.4)\n";
      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR002";
      return fn_ret;
    }
    answer = floor(stod(answer_buffer[0]));
  }
  else if (valid_func == "ceil")
  {
    if (answer_buffer.size() != 1)
    {
      cout << "Invalid number of inputs for ceil, must be one argument, Ex: ceil(2.6)\n";
      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR002";
      return fn_ret;
    }
    answer = ceil(stod(answer_buffer[0]));
  }
  else if (valid_func == "round")
  {
    if (answer_buffer.size() != 1)
    {
      cout << "Invalid number of inputs for round, must be one argument, Ex: round(-4.5)\n";
      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR002";
      return fn_ret;
    }
    answer = round(stod(answer_buffer[0]));
  }
  else if (valid_func == "trunc")
  {
    if (answer_buffer.size() != 1)
    {
      cout << "Invalid number of inputs for trunc, must be one argument, Ex: trunc(-5.6)\n";
      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR002";
      return fn_ret;
    }
    answer = trunc(stod(answer_buffer[0]));
  }
  else if (valid_func == "mean")
  {
    if (answer_buffer.size() != 0)
    {
      double mean_sum = 0;
      for (const auto &ans_buff : answer_buffer)
      {
        mean_sum += stod(eval(ans_buff));
      }
      answer = mean_sum / answer_buffer.size();
    }
  }
  else if (valid_func == "median")
  {
    if (answer_buffer.size() == 1)
    {
      answer = stod(answer_buffer[0]);
    }
    else if (answer_buffer.size() != 0)
    {
      double median, median_buff;
      int med_vec_size, med_vec_index;
      vector<double> median_vec;

      for (const auto &ans_buff : answer_buffer)
      {
        median_vec.push_back(stod(eval(ans_buff)));
      }

      med_vec_size = median_vec.size();

      sort(median_vec.begin(), median_vec.end());

      if (med_vec_size % 2 == 0)
      {
        med_vec_index = (med_vec_size / 2) - 1;
        answer = (median_vec[med_vec_index] + median_vec[med_vec_index + 1]) / 2;
      }
      else
      {
        med_vec_index = (med_vec_size / 2);
        answer = median_vec[med_vec_index];
      }
    }
  }
  else if (valid_func == "mode")
  {
    vector<double> mode_vec;
    vector<int> mode_vec_int;
    double buffer_eval;
    bool append;
    int greatest = 0;
    string special_val;

    for (const auto &buffer : answer_buffer)
    {
      buffer_eval = stod(eval(buffer));

      if (empty(mode_vec))
      {
        mode_vec.push_back(buffer_eval);
        mode_vec_int.push_back(1);
      }
      else
      {
        for (int x = 0; x < mode_vec.size(); x++)
        {
          if (buffer_eval == mode_vec[x])
          {
            append = true;
            mode_vec_int[x]++;
          }
        }
        if (!append)
        {
          mode_vec.push_back(buffer_eval);
          mode_vec_int.push_back(1);
        }
        append = false;
      }
    }
    for (int x = 0; x < mode_vec_int.size(); x++)
    {
      if (mode_vec_int[x] > greatest)
      {
        greatest = mode_vec_int[x];
      }
    }
    cout << "mode/s -> ";
    for (int x = 0; x < mode_vec.size(); x++)
    {
      if (mode_vec_int[x] == greatest)
      {
        if (double(int(mode_vec[x])) == mode_vec[x])
        {
          if (!(x == mode_vec.size() - 1))
          {
            special_val += to_string(int(mode_vec[x])) + " ";
          }
          else
          {
            special_val += to_string(int(mode_vec[x]));
          }
        }
        else
        {
          if (!(x == mode_vec.size() - 1))
          {
            special_val += to_string(mode_vec[x]) + " ";
          }
          else
          {
            special_val += to_string(mode_vec[x]);
          }
        }
      }
    }
    fn_ret.special = "no_val";
    fn_ret.special_val = "[" + special_val + "]";
  }
  fn_ret.answer = answer;
  fn_ret.index = x;

  return fn_ret;
}
string analyze(vector<string> buffer, string var_buffer)
{
  vector<string> functions;
  string analyze_buffer;
  for (int x = 0; x < var_buffer.size(); x++)
  {
    if (is_a_z(var_buffer[x]))
    {
      while (is_a_z(var_buffer[x]))
      {
        analyze_buffer += var_buffer[x];
        x++;
      }
      if (var_buffer[x] == '(')
      {
        analyze_buffer = "";
        continue;
      }
      functions.push_back(analyze_buffer);
    }
    analyze_buffer = "";
  }
  if (empty(functions))
  {
    return "valid";
  }
  for (int x = 0; x < buffer.size(); x++)
  {
    for (int y = 0; y < functions.size(); y++)
    {
      if (buffer[x] == functions[y])
      {
        return "ERR006 - Circular equation, an equation is calling itself\n";
      }
      else
      {
        if (variables.find(functions[y]) != variables.end())
        {
          buffer.push_back(functions[y]);
          if (analyze(buffer, variables[functions[y]]) != "valid")
          {
            return "ERR006 - Circular equation, an equation is calling itself\n";
          }
          buffer.pop_back();
        }
      }
    }
  }
  return "valid";
}
pair<string, int> get_tokens_isdigit(string inpt, int x)
{
  int period_count = 0;
  string buffer;

  if (inpt[x] == '.')
  {
    period_count++;
  }
  buffer += inpt[x++];
  while (isdigit(inpt[x]) || inpt[x] == '.')
  {
    if (inpt[x] == '.')
    {
      period_count++;
    }
    buffer += inpt[x];
    x++;
  }
  x--;
  if (period_count > 1)
  {
    cout << "ERR004 - Invalid Number of Decimals";
    return make_pair("invalid", -1);
  }
  return make_pair(buffer, x);
}
vector<vector<string>> get_tokens(string inpt)
{
  // char of inputted string loop
  vector<string> operands, operators, vec_buffer;
  vector<vector<string>> to_return;
  string buffer, valid_func, var_buffer, var_to_oper;
  int right_par, left_par, x = 0;
  char inptx;
  bool up_x = false, neg_1 = false;

  inpt = remove_spaces(inpt);

  if (inpt[0] == '+' || inpt[0] == '-')
  {
    if (inpt[0] == '-')
    {
      neg_1 = true;
    }
    x++;
  }
  while (x < inpt.length())
  {
    // isdigit
    if (isdigit(inpt[x]) || inpt[x] == '.')
    {
      pair<string, int> token = get_tokens_isdigit(inpt, x);

      if (token.first == "invalid")
      {
        cout << "ERR004 - Invalid Number of Decimals";
        return {{"invalid"}, {"invalid"}};
      }
      operands.push_back(token.first);
      x = token.second;
    }
    else if (is_classic_operator(inpt[x]))
    {
      buffer += inpt[x];
      if (is_classic_operator(inpt[++x]))
      {
        buffer += inpt[x];
        if (is_classic_operator(inpt[++x]))
        {
          cout << "Invalid Input, the program only allows one to two operators "
                  "one after the other";
          return {{"invalid"}, {"invalid"}};
        }
        else
        {
          x--;
          if (is_valid_operator(buffer))
          {
            operators.push_back(buffer);
            buffer = "";
          }
          else
          {
            cout << "Invalid Operator " << buffer;
            return {{"invalid"}, {"invalid"}};
          }
        }
      }
      else
      {
        operators.push_back(buffer);
        buffer = "";
        x--;
      }
    }
    else if (is_a_z(inpt[x]))
    {
      buffer += inpt[x];
      while (is_a_z(inpt[++x]))
      {
        buffer += inpt[x];
      }
      if (inpt[x] == '(')
      {
        valid_func = is_buffer_valid_func(buffer);
        if (valid_func == "invalid")
        {
          cout << "Invalid Function" << buffer;
          return {{"invalid"}, {"invalid"}};
        }
        else
        {
          fn_return evaluated_func = func_eval(buffer, x + 1, inpt, valid_func);
          if (evaluated_func.special == "no_val")
          {
            return {{"no_val"}, {evaluated_func.special_val}};
            if (evaluated_func.error != "")
            {
              return {{"invalid"}, {"invalid"}};
            }
            x = evaluated_func.index;
            operands.push_back(to_string(evaluated_func.answer));
            buffer = "";
          }
        }
      }
      else if (inpt[x] == '=')
      {
        if (buffer == "ans")
        {
          cout << "Invalid input, ans is a reserved word\n";
          return {{"invalid"}, {"invalid"}};
        }
        if (!empty(operands) || !empty(operators))
        {
          cout << "Invalid input, do not create variables in the same line as calcuations\n";
          return {{"invalid"}, {"invalid"}};
        }
        if (variables.find(buffer) != variables.end())
        {
          for (int y = ++x; y < inpt.length(); y++)
          {
            if (inpt[y] == '=')
            {
              cout << "Invalid equation, more than 1 = sign\n";
              return {{"invalid"}, {"invalid"}};
            }
            var_buffer += inpt[y];
          }
          vec_buffer.push_back(buffer);
          if (analyze(vec_buffer, var_buffer) != "valid")
          {
            cout << "Invalid equation\n";
            return {{"invalid"}, {"invalid"}};
          }
          variables[buffer] = var_buffer;
          operands.push_back("changed variable");
          operators.push_back("changed variable");
        }
        else
        {
          for (int y = ++x; y < inpt.length(); y++)
          {
            if (inpt[y] == '=')
            {
              cout << "Invalid equation, more than 1 = sign\n";
              return {{"invalid"}, {"invalid"}};
            }
            var_buffer += inpt[y];
          }
          vec_buffer.push_back(buffer);
          if (analyze(vec_buffer, var_buffer) != "valid")
          {
            cout << "Invalid equation\n";
            return {{"invalid"}, {"invalid"}};
          }
          variables.insert({buffer, var_buffer});
          operands.push_back("added variable");
          operators.push_back("added variable");
        }
        buffer = "";
        break;
      }
      else
      {
        x--;
        if (variables.find(buffer) != variables.end())
        {
          var_to_oper = variables[buffer];
          var_to_oper = eval(var_to_oper);
          if (var_to_oper == "(ERROR)")
          {
            return {{"invalid"}, {"invalid"}};
          }

          operands.push_back(var_to_oper);

          buffer = "";
        }
        else
        {
          cout << "ERR003 - variable not found '" << buffer << "'\n";
          return {{"invalid"}, {"invalid"}};
        }
      }
    }
    else if (inpt[x] == '(')
    {
      left_par = 1;
      right_par = 0;
      while (right_par != left_par)
      {
        x++;
        if (inpt[x] == '(')
        {
          left_par++;
        }
        else if (inpt[x] == ')')
        {
          right_par++;
          if (right_par == left_par)
          {
            break;
          }
        }
        buffer += inpt[x];
      }

      buffer = "par" + buffer;
      operands.push_back(buffer);
      buffer = "";
    }
    else
    {
      cout << "Invalid symbol " << inpt[x] << "\n";
      return {{"invalid"}, {"invalid"}};
    }
    x++;
  }
  if (neg_1)
  {
    operands[0] = "-" + operands[0];
  }
  to_return.push_back(operators);
  to_return.push_back(operands);
  return to_return;
}
string eval(string inpt)
{
  vector<vector<string>> test = get_tokens(inpt);
  vector<string> operators = test[0];
  vector<string> operands = test[1];

  if (empty(operands))
  {
    return "no operands";
  }
  if (!empty(operands) && !empty(operators))
  {
    if (operands[0] == "invalid" && operators[0] == "invalid")
    {
      return "(ERROR)";
    }
    else if (operators[0] == "added variable")
    {
      return "added variable";
    }
    else if (operators[0] == "changed variable")
    {
      return "changed variable";
    }
    else if (operators[0] == "no_val")
    {
      return operands[0];
    }
  }
  if (operands[0][0] == '[')
  {
    return operands[0];
  }
  if (operands[0] == "no operands")
  {
    return "no operands";
  }
  if (operands.size() != operators.size() + 1)
  {
    return "ERR001 - Invalid Number of Operands/Operators";
  }

  for (int x = 0; x < operands.size(); x++)
  {
    if (operands[x].length() > 3)
    {
      if (operands[x].substr(0, 3) == "par")
      {
        operands[x] = eval(operands[x].substr(3));
      }
    }
  }

  for (const auto &oper : operands)
  {
    if (oper == "undefined")
    {
      return "undefined";
    }
  }

  while (operators.size() > 0)
  {
    for (int x = 0; x < operators.size(); x++)
    {
      if (operators[x] == "*" || operators[x] == "*-" || operators[x] == "*+")
      {
        if (operators[x] == "*")
        {
          operands[x] = to_string(stod(operands[x]) * stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "*-")
        {
          operands[x] = to_string(stod(operands[x]) * stod("-" + operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "*+")
        {
          operands[x] = to_string(stod(operands[x]) * stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        operators.erase(operators.begin() + x);
        x--;
      }
      if (operators[x] == "/" || operators[x] == "/-" || operators[x] == "/+")
      {
        if (operands[x + 1] == "0")
        {
          return "undefined";
        }
        if (operators[x] == "/")
        {
          operands[x] = to_string(stod(operands[x]) / stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "/-")
        {
          operands[x] = to_string(stod(operands[x]) / stod("-" + operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "/+")
        {
          operands[x] = to_string(stod(operands[x]) / stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        operators.erase(operators.begin() + x);
        x--;
      }
    }
    for (int x = 0; x < operators.size(); x++)
    {
      if (operators[x] == "+" || operators[x] == "+-" || operators[x] == "++")
      {
        if (operators[x] == "+")
        {
          operands[x] = to_string(stod(operands[x]) + stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "+-")
        {
          operands[x] = to_string(stod(operands[x]) - stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "++")
        {
          operands[x] = to_string(stod(operands[x]) + stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        operators.erase(operators.begin() + x);
        x--;
      }
      if (operators[x] == "-" || operators[x] == "--" || operators[x] == "-+")
      {
        if (operators[x] == "-")
        {
          operands[x] = to_string(stod(operands[x]) - stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "--")
        {
          operands[x] = to_string(stod(operands[x]) + stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "-+")
        {
          operands[x] = to_string(stod(operands[x]) - stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        operators.erase(operators.begin() + x);
        x--;
      }
    }
  }

  if (stod(operands[0]) - stoi(operands[0]) == 0)
  {
    operands[0] = to_string(stoi(operands[0]));
  }
  return operands[0];
}
int main()
{
  cout << "=================\nWelcome to InCalc\n=================\n\n";
  menu_loop();

  return 0;
}
