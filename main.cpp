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

using namespace std;

namespace fs = filesystem;
map<string, string> variables;

// misc
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
    cout << endl;
  }
}
void clear_screen()
{
  system("CLS");
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
    if (outpt[x] != 0)
    {
      first_trailing = 0;
      found == false;
    }
    else if (found == false)
    {
      if (outpt[x] == 0)
      {
        first_trailing = x;
        found == true;
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
        cout << "\b \b";
        password.pop_back();
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
    if (filenames[x].length() > 5)
    {
      users_buffer = "";
      for (int y = 0; y < filenames[x].length() - 5; y++)
      {
        char z = filenames[x][y + 6];
        if (filenames[x][y + 6] == ' ' || filenames[x][y + 6] == '\\' ||
            filenames[x][y + 6] == 0)
        {
          break;
        }
        users_buffer += filenames[x][y + 6];
      }
      if (string(users_buffer) != string(prev_buffer))
      {
        users.push_back(users_buffer);
      }
      prev_buffer = users_buffer;
    }
  }

  int y = 0;

  return users;
}
vector<string> scan_users()
{
  fs::path p = fs::current_path();
  vector<string> filenames, filenames_files, users;
  string buffer, files_lit_str = "files";

  for (const auto &entry : fs::recursive_directory_iterator(p))
  {
    buffer = entry.path().string();
    filenames.push_back(buffer.substr(p.string().length() + 1));
  }

  for (int x = 0; x < filenames.size(); x++)
  {
    if (filenames[x].substr(0, 5) != "files")
    {
      filenames.erase(filenames.begin() + x);
      x--;
    }
  }

  users = users_sort(filenames);

  return users;
}
vector<string> scan_files(string username)
{
  fs::path p = fs::current_path();
  vector<string> filenames;
  string buffer, files_lit_str = "files";
  p = p / "files/" / username;

  for (const auto &entry : fs::recursive_directory_iterator(p))
  {
    buffer = entry.path().string();
    filenames.push_back(buffer.substr(p.string().length() + 1));
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
      // cout << filenames[x][y] << endl;
      if (filenames[x][y] == '\\')
      {
        break;
      }
      buffer += filenames[x][y];
    }
    if (buffer.length() <= 4)
    {
      // cout << buffer;
      if (empty(dir))
      {
        dir.push_back(buffer);
      }
      else if (!(buffer == dir[dir_index]))
      {
        dir.push_back(buffer);
        dir_index++;
      }
      // cout << "dir push" << endl;
    }
    else if (buffer.substr(buffer.length() - 4, 4) == ".txt")
    {
      file.push_back(buffer);
      // cout << "file push" << endl;
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
      // cout << "dir push" << endl;
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
    stream.insert(stream.begin(), x.first + ";" + x.second);
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
          if (stream[x][y] == ';')
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
    stream.push_back(">>> " + inpt_stream);
    oupt_stream = eval(inpt_stream);

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
  if (c == 224)
  {
    c = _getch();
    switch (c)
    {
    case 72:
      return "UP";
    case 80:
      return "DOWN";
    }
  }
  else if (c == 13)
  {
    return "ENTER";
  }
  else if (c == 3)
  {
    abort();
  }
  else if (c == 97)
  {
    return "add";
  }
  else if (c == 100)
  {
    return "delete";
  }
  else if (c == 113)
  {
    return "quit";
  }
  else if (c == 109)
  {
    return "mkdir";
  }
  else
  {
    return "please_warning";
  }
  return "INVALID";
}
void menu_file_sel(string username)
{
  vector<string> filenames;
  bool invalid_input = false;

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
      cout << "\n - UP and DOWN arrow keys to navigate\n - ENTER -> enter file or directory\n - 'a' -> add file \n - 'd' -> delete file\n - 'm' -> make directory\n - 'q' -> quit or escape\n";
    }
    else if (invalid_input)
    {
      invalid_input = false;
      cout << "\n - UP and DOWN arrow keys to navigate\n - ENTER -> enter file or directory\n - 'a' -> add file \n - 'd' -> delete file\n - 'm' -> make directory\n - 'q' -> quit or escape\n";
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
    else
    {
      invalid_input = true;
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
  char pw_buffer;
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
  char pw_buffer;
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
             << "type quit() to exit" << endl;
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
}
void menu_loop()
{
  string log_sign_str[2] = {"Login", "Sign Up"};
  string arrow_inpt;

  int pos = 0;
  while (true)
  {
    for (int x = 0; x < 2; x++)
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
    if (arrow_inpt == "please_warning")
    {
      cout << "(Use the Up and Down arrow keys to select and the Enter key to "
              "proceed | press q to quit)"
           << endl;
    }

    arrow_inpt = menu_loop_reader();

    if (arrow_inpt == "UP")
    {
      pos = 0;
    }
    else if (arrow_inpt == "DOWN")
    {
      pos = 1;
    }
    else if (arrow_inpt == "ENTER")
    {
      if (pos == 0)
      {
        clear_screen();
        loginform();
      }
      else
      {
        clear_screen();
        regform();
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
    if (x != ' ')
    {
      buffer += x;
    }
  }
  return buffer;
}
bool is_a_z(char inpt)
{
  int cha_ascii = int(inpt);
  if (cha_ascii >= 97 && cha_ascii <= 122)
  {
    return true;
  }
  return false;
}

string is_buffer_valid_func(string inpt)
{
  bool valid = false;
  string test[5] = {"pow", "sqrt"};
  for (int x = 0; x < 5; x++)
  {
    if (inpt == test[x])
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
};
fn_return func_eval(string buffer, int x, string inpt, string valid_func)
{
  int par_left = 1;
  int par_right = 0;
  int index = 0;
  fn_return fn_ret;
  vector<string> answer_buffer;
  double answer;

  fn_ret.error = "";

  answer_buffer.push_back("");
  while (par_left != par_right)
  {
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
    if (answer_buffer.size() == 2)
    {
      answer = pow(stod(answer_buffer[0]), stod(answer_buffer[1]));
    }
    else
    {
      cout << "Invalid Number of Inputs for pow, must be two inputs, Ex: pow(3, 2)\n";
      fn_ret.answer = 0;
      fn_ret.index = --x;
      fn_ret.error = "ERR002";
      return fn_ret;
    }
  }
  else if (valid_func == "sqrt")
  {
    if (answer_buffer.size() == 1)
    {
      if (stod(answer_buffer[0]) < 0)
      {
        cout << "Invalid input for sqrt, sqrt is unable to process negative numbers\n";
        fn_ret.answer = 0;
        fn_ret.index = --x;
        fn_ret.error = "ERR005";
      }
      answer = sqrt(stod(answer_buffer[0]));
    }
    else
    {
      cout << "Invalid number of inputs for sqrt, must be one input, Ex: sqrt(2)\n";
    }
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
vector<vector<string>> get_tokens(string inpt)
{
  // char of inputted string loop
  vector<string> operands, operators, vec_buffer;
  vector<vector<string>> to_return;
  string buffer, valid_func, var_buffer, var_to_oper;
  int right_par, left_par, period_count, x = 0;
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
    period_count = 0;
    if (isdigit(inpt[x]) || inpt[x] == '.')
    {
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
        return {{"invalid"}, {"invalid"}};
      }
      operands.push_back(buffer);
      buffer = "";
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
          if (evaluated_func.error != "")
          {
            return {{"invalid"}, {"invalid"}};
          }
          x = evaluated_func.index;
          operands.push_back(to_string(evaluated_func.answer));
          buffer = "";
        }
      }
      else if (inpt[x] == '=')
      {
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
        }
        else
        {
          buffer += inpt[x];
        }
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
          operands[x] = "-" + to_string(stod(operands[x]) * stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "*+")
        {
          operands[x] = to_string(stod(operands[x]) * stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        operators.erase(operators.begin() + x);
      }
    }
    for (int x = 0; x < operators.size(); x++)
    {
      if (operands[x + 1] == "0")
      {
        return "undefined";
      }
      if (operators[x] == "/" || operators[x] == "/-" || operators[x] == "/+")
      {
        if (operators[x] == "/")
        {
          operands[x] = to_string(stod(operands[x]) / stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "/-")
        {
          operands[x] = "-" + to_string(stod(operands[x]) / stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "/+")
        {
          operands[x] = to_string(stod(operands[x]) / stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        operators.erase(operators.begin() + x);
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
          operands[x] = "-" + to_string(stod(operands[x]) - stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "++")
        {
          operands[x] = to_string(stod(operands[x]) + stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        operators.erase(operators.begin() + x);
      }
    }
    for (int x = 0; x < operators.size(); x++)
    {
      if (operators[x] == "-" || operators[x] == "--" || operators[x] == "-+")
      {
        if (operators[x] == "-")
        {
          operands[x] = to_string(stod(operands[x]) - stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "--")
        {
          operands[x] = "-" + to_string(stod(operands[x]) + stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        else if (operators[x] == "-+")
        {
          operands[x] = to_string(stod(operands[x]) - stod(operands[x + 1]));
          operands.erase(operands.begin() + x + 1);
        }
        operators.erase(operators.begin() + x);
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
  menu_loop();

  return 0;
}
