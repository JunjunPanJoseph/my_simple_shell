<!DOCTYPE html PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<!-- saved from url=(0070)https://cgi.cse.unsw.edu.au/~cs1521/18s2/assignments/assign2/index.php -->
<html><head><meta http-equiv="Content-Type" content="text/html; charset=windows-1252">
<title>COMP1521 18s2 - Assignment 2</title>
<link rel="stylesheet" type="text/css" href="./COMP1521 18s2 - Assignment 2_files/course.css"><style></style></head>
<body>
<div align="center">
<table width="100%" border="0">
<tbody><tr valign="top">
<td align="left" width="25%">
  <span class="tiny"><a href="https://www.cse.unsw.edu.au/~cs1521/18s2/index.php">COMP1521 18s2</a></span>
</td>
<td align="center" width="50%">
  <span class="heading">Assignment 2</span><br>
  <span class="subheading">My Very Own Shell</span>
</td>
<td align="right" width="25%">
  <span class="tiny"><a href="https://www.cse.unsw.edu.au/~cs1521/18s2/index.php">Computer System Fundamentals</a></span>
</td>
</tr></tbody></table>
</div>
<h2>Objectives</h2>
<ul>
<li> to give you experience writing C code to manipulate processes
</li><li> to give you experience with interprocess communication (pipes)
</li><li> to give you further experience with data structures in C
</li></ul>

<h2>Admin</h2>
<table border="0" cellpadding="4">
<tbody><tr valign="top"><td><b>Marks</b></td><td>11 <small>(towards total course mark)</small></td></tr>
<tr valign="top"><td><b>Group?</b></td><td>This assignment is completed <b>individually</b></td></tr>
<tr valign="top"><td><b>Due</b></td><td>by 11:59:59pm on Sunday 7th October</td></tr>
<tr valign="top"><td><b>Submit</b></td><td><tt>give cs1521 ass2 mymysh.c history.h history.c</tt> &nbsp; or via Webcms<br>
</td></tr><tr valign="top"><td><b>Late<br>Penalty</b></td><td>0.09 marks per hour late (approx 2.2 marks per day) off the ceiling<br><small>(e.g. if you are 36 hours late, your maximum possible mark is 7.8/11)</small></td></tr>
<tr valign="top"><td><b>Assessment</b></td><td>
For a guide to style, use the code in the lectures and tute solutions, and the supplied code.
<table cellpadding="6">
<tbody><tr valign="top">
 <td>9&nbsp;marks</td>
 <td>for correct performance; measured by auto-testing on a range of test cases,
   some of which will be provided for you to test your code as you write it</td>
</tr>
<tr valign="top">
 <td>1&nbsp;mark</td>
 <td>for commenting the code; you don't need a comment on every line,
   but roughly one comment on each block of C statements that does a meaningful task</td>
</tr>
<tr valign="top">
 <td>1&nbsp;mark</td>
 <td>for readable code; sensible names, consistent use of indentation to highlight
   control structures</td>
</tr>
</tbody></table>
<p>
If your submitted code won't compile, your maximum possible "performance" mark is 3/9.
The solution: make sure your code compiles and runs under <tt>dcc</tt> before
submitting.
If your submitted code fails all the performance tests, your maximum possible
performance mark is 5/9.
In both cases, the actual mark will be determined by a tutor's assessment
on how close your code is to working.
</p>
</td></tr>
</tbody></table>

<h2>Background</h2>
<p>
A shell is a program that executes other programs.
Command-line based shells (e.g. <tt>bash</tt>) read lines
of text, break each line into tokens, and execute the command
indicated by the first token.
For example:
</p>
<pre>ls -al ~cs1521/bin
</pre>
<p>
is a command with three tokens ("<tt>ls</tt>", "<tt>-al</tt>"
and "~cs1521/bin").
The shell works out where the executable for the <tt>ls</tt>
command is located, and executes it, passing the second two
tokens as command-line arguments.
</p>
<p>
However, shells do a lot more than just reading command lines
and executing them. For example, they keep a history of previous
commands to make it easy to re-execute them. They also allow
users to capture the output of a command, by redirecting its
output into a file. And, importantly on Unix/Linux, they allow
users to build a pipeline of commands to achieve powerful
effects without having to write a program.
For example, the following pipeline produces a list of the
top ten most frequently used words in a text file:
</p>
<pre>cat blah.txt | tr -cs '[a-z]' '\n' | sort | uniq -c | sort -nr | head -10
</pre>
<p>
For more details on Unix/Linux commands and how they can be
combined in this way, see the COMP2041 web site.
</p>
<p>
Unix/Linux shells also typically provide a full programming
capability, if pipelines aren't quite enough.
</p>
<p>
You will not be required to implement a shell as powerful
as <tt>bash</tt> for this assignment, but you will build
some of the core features.
</p>

<h2>Setting Up</h2>
<p>
Create a private directory for doing the assignment, and put the
assignment files in it by running the following command:
</p>
<pre>$ <b>unzip <a href="https://www.cse.unsw.edu.au/~cs1521/18s2/assignments/assign2/assign2.zip">/home/cs1521/web/18s2/assignments/assign2/assign2.zip</a></b>
</pre>
<p>
If you're working on this at home, download the ZIP file and
create the files on your home machine.
It's fine to work on your own machine but remember to
<em>always</em> test your code on the CSE machines before
submitting.
</p>
<p>
The above command will create the following files:
</p>
<dl>
<dt><tt>Makefile</tt></dt>
<dd><p>
A file to control compilation of the various modules in the system.
</p></dd>
<dt><tt>mymysh.c</tt></dt>
<dd><p>
A skeleton for the shell program that you are required to write.
</p></dd>
<dt><tt>history.h</tt></dt>
<dd><p>
A complete interface for the functions on history list data.
</p></dd>
<dt><tt>history.c</tt></dt>
<dd><p>
A skeleton for the history list functions.
</p></dd>
</dl>

<h2>Exercise</h2>
<p>
The aim of this exercise is to complete the supplied program skeleton
in <tt>mymysh.c</tt>, giving an executable called <tt>./mymysh</tt>.
This requires you to implement the command history list data structures
and functions, and then use these in implementing the main program.
You can add as many functions as you like to <tt>mymysh.c</tt> and
<tt>history.c</tt> files.
</p>
<p>
The shell needs to be able to do the following:
</p>
<dl>
<dt><b>Read and execute commands</b> <small>(1 mark)</small></dt>
<dd>
<p>
The shell should print a prompt using the supplied <tt>prompt()</tt>
function. It then reads a single line of text and interprets it as
a command.
A command is a sequence of space-separated tokens on a single line.
The first token is treated as the name of a command, where the
command exists as an executable file somewhere in the user's <tt>PATH</tt>.
If no such executable is found, the shell should print a
<nobr>"<tt>Command not found</tt></nobr>" message.
</p>
<p>
The command is invoked via the <tt>execve()</tt> library function, 
with the full pathname of the command as the first parameter, the sequence
of tokens as the second parameter, and the user's environment (from the
third argument of the the main program) as the third parameter.
This is similar as the Week 08 Lab (except that the lab didn't use
the available environment).
</p>
<p>
One difference to the Lab is that the shell should print some
additional information before and after the output fom the command.
Before the command it should show the pathname of the command
executable: <span style="color:#CC0000">use the full pathname
if the command is found by using a search of the PATH; if the
command name is relative, use the name as given.</span>
After the command, it should print the command's return
status <span style="color:#CC0000">(as determined using WEXITSTATUS)</span>.
The output from the command should be delineated by twenty
hyphens, as shown below:
</p>
<pre>mymysh$ <b>ls -l</b>
Running /bin/ls ...
--------------------
<span class="comment">... output from the ls -l command ...</span>
--------------------
Returns 0
mymysh$
</pre>
<p>
Note that "<tt>mymysh$</tt>" is the shell's prompt.
</p>
<p>
There are a number of built-in "commands" that are not executed
as described above, but are handled directly by the shell.
The built-ins are described below.
</p>
<p>
If the command line has no tokens, it is simply ignored and
a new shell prompt is printed.
</p>
</dd><dt><b>Maintain a history of the previous 20 valid commands</b> <small>(2 marks)</small></dt>
<dd><p>
The shell should maintain a persistent list of the most recent 20
valid commands that the shell has executed. Each command is associated
with a sequence number; sequence numbers increase constantly over time,
and persist between sessions with the shell (see the examples below).
</p>
<p>
While shell is executing, the command history is maintained in a
fixed-size data structure defined in <tt>history.c</tt>.
A number of interface functions are defined on this list and
should be used in the <tt>main()</tt> function.
If you asbolutely cannot stand the supplied data structures and
functions, feel free to define your own. As long as the history
behaves as required, the precise implementation of the command
history is not critical.
</p>
<p>
The command history has to persist between executions of the
<tt>mymysh</tt> program. To achieve this, it saves the history
in a file <tt>$HOME/.mymysh_history</tt> when <tt>mymysh</tt>
terminates and restores it from this file when it next starts.
The <tt>.mymysh_history</tt> is simply a text file, containing
the most recent 20 commands and their sequence numbers.
<span style="color:#CC0000">
It must be in the same format as that produced by the <tt>mymysh</tt>
binary (i.e. <tt>"&#9141;%3d&#9141;&#9141;%s\n"</tt>) which is the same format as
used to display the history within the shell (where &#9141; represents a single
space character).
</span>
</p>
<p>
Commands from the history can be re-executed by using the
special notation <tt>!<i>SeqNo</i></tt> and giving the
sequence number for one of the commands in the history.
The command from the history should become the current
command and then be treated as if it had been typed by
the user. The special notation <tt>!!</tt> re-executes
the previous command.
</p><p>
Note that, unlike most Unix/Linux shells, <tt>mymysh</tt> does not
place invalid commands in the history, so commands should be checked
for the following before being executed:
</p>
<ul>
<li> an executable for the command (first token) actually exists </li>
<li> stdin is redirected, but without giveng a filename to read from </li>
<li> stdin is redirected, but with a filename that is nonexistent or not readable </li>
<li> stdout is redirected, but without giving a filename to write to </li>
<li> stdout is redirected, but with a filename that is not writeable </li>
<li> using <tt>!<i>SeqNo</i></tt> but with an invalid sequence number </li>
</ul>
<p>
If the command line produces any of the above errors,
it should not be placed in the history.
</p></dd>
<dt><b>Implement shell built-in commands</b> <small>(1 mark)</small></dt>
<dd><p>
The following commands are handled by the shell, and do not need
to be searched for in the command path.
</p>
<dl>
<dt><b>exit</b></dt>
<dd><p>terminate the shell (after saving the command history) </p></dd>
<dt><b>h</b> or <b>history</b></dt>
<dd><p>display the last 20 commands, with their sequence numbers</p></dd>
<dt><b>pwd</b></dt>
<dd><p>print the shell's current working directory<br>
     <small>(hint: <code>getcwd()</code>)</small></p></dd>
<dt><b>cd</b> <i>Directory</i></dt>
<dd> <p>change the shell's working directory (after filename expansion)<br>
     after changing, show the new working directory<br>
     <small>(hint: <code>chdir()</code> and <code>getcwd()</code>)</small> </p></dd>
</dl>
<p>
The <tt>exit</tt> built-in is not placed in the command history.
</p></dd>
<dt><b>Expand filename wildcards</b> <small>(2 marks)</small></dt>
<dd><p>
If any of the following characters
(<tt>'*'</tt>, <tt>'?'</tt>, <tt>'['</tt>, <tt>'~'</tt>)
appears in one of the tokens, that token should be replaced by
all of the tokens matching that token using the <tt>glob()</tt>
library function. This may result in the tokens list becoming
longer than initially. If there are no matches, use the token
unchanged. This should be done before any of the actions
described below.
<br><small>(hint: use <code>GLOB_NOCHECK|GLOB_TILDE</code>
as the second parameter of the <code>glob()</code> function)</small>
</p></dd>
<dt><b>Redirect command input</b> <small>(2 marks)</small></dt>
<dd><p>
If the command line contains the tokens <tt>&lt;</tt> and a filename
as the last two tokens, the command should be executed with its
standard input connected to the named file.
If the file does not exist, or is not readable, that is an error.
Having <tt>&lt;</tt> as the last token, or elsewhere in
the command-line is also an error.<br>
<small>(Hint: <tt>pipe()</tt> and <tt>dup2()</tt>)</small>
</p></dd>
<dt><b>Redirect command output</b> <small>(1 mark)</small></dt>
<dd><p>
If the command line contains the tokens <tt>&gt;</tt> and a filename
as the last two tokens, the command should be executed with its
standard output connected to the named file.
If the file does not already exist or exists and is writeable,
then it is truncated to zero length and it current contents
are overwritten.
If the file exists and is not writeable, that is an error.
Having <tt>&gt;</tt> as the last token, or elsewhere in
the command-line is also an error.<br>
<small>(Hint: <tt>pipe()</tt> and <tt>dup2()</tt>)</small>
</p>
<p>
<span style="color:#CC0000">
Shell built-ins cannot have their output redirected.
</span>
</p></dd>
</dl>
<h3>Hints</h3>
<p>
The main program of <tt>mymysh</tt> should be structured roughly
as follows:
</p>
<pre>main() {
   restore the command history
   print prompt
   while (more commands) {
      if empty command, ignore
      handle ! history substitution 
      tokenise
      handle *?[~ filename expansion
      handle shell built-ins
      check for input/output redirections
      find executable using first token
      if none, then Command not found
      sort out any redirections
      run the command
      print prompt
   }
   save command history
}
</pre>
<p>
Trying to implement the whole of the above at once is difficult.
I'd suggest implementing it in stages:
</p>
<ul>
<li> get "normal" commands running properly (Week 08 Lab)
</li><li> then add shell built-ins
</li><li> then add command history
</li><li> then add history substitution
</li><li> then add filename expansion
</li><li> then add input/output redirection
</li></ul>
<p>
Marks are available for each of the components. You can get full
marks for a working component, even if other components don't work
or aren't implemented.
</p>
<p>
The following example shows how the shell should work in practice:
</p>
<pre>$ <b>./mymysh</b>
mymysh$ <span style="color:red;font-weight:bold">ls</span>
Running /bin/ls ...
--------------------
Makefile   history.h  mymysh	mymysh.o   xxx
history.c  history.o  mymysh.c	mymysh1.c
--------------------
Returns 0
mymysh$ <span style="color:red;font-weight:bold">wc -l *.c</span>
Running /usr/bin/wc ...
--------------------
  129 history.c
  423 mymysh.c
  470 mymysh1.c
 1022 total
--------------------
Returns 0
mymysh$ <span style="color:red;font-weight:bold">cat xyz</span>
Running /bin/cat ...
--------------------
cat: xyz: No such file or directory
--------------------
Returns 1
mymysh$ <span style="color:red;font-weight:bold">cat &lt; xyz</span>
<span class="comment">... note: not recorded in history ...</span>
Input redirection: No such file or directory
mymysh$ <span style="color:red;font-weight:bold">ls -l &gt; xyz</span>
Running /bin/ls ...
--------------------
--------------------
Returns 0
mymysh$ <span style="color:red;font-weight:bold">cat xyz</span>
Running /bin/cat ...
--------------------
total 76
-rw-r--r-- 1 jas jas   303 Sep 16 21:59 Makefile
-rw-r--r-- 1 jas jas  3310 Sep 16 21:01 history.c
-rw-r--r-- 1 jas jas   377 Sep 16 21:02 history.h
-rw-r--r-- 1 jas jas  2640 Sep 16 22:41 history.o
-rwxr-xr-x 1 jas jas 17216 Sep 16 22:41 mymysh
-rw-r--r-- 1 jas jas 11242 Sep 16 22:36 mymysh.c
-rw-r--r-- 1 jas jas  7792 Sep 16 22:41 mymysh.o
-rw-r--r-- 1 jas jas 12734 Sep 16 20:54 mymysh1.c
-rw-r--r-- 1 jas jas   301 Sep 16 22:40 xxx
-rw-r--r-- 1 jas jas     0 Sep 16 22:48 xyz
--------------------
Returns 0
mymysh$ <span style="color:red;font-weight:bold">h</span>
  11  make
  12  ls
  13  make clean
  14  cat xxx
  15  ls -l &gt; xxx
  16  cat xxx
  17  echo Ooops ... mymysh executable is gone
  18  make
  19  h
  20  cd ~cs1521
  21  ls
  22  cd web/18s2
  23  ls
  24  h
  25  h
  26  ls
  27  wc -l *.c
  28  cat xyz
  29  ls -l &gt; xyz
  30  cat xyz
mymysh$ <span style="color:red;font-weight:bold">exit</span>
<span class="comment">... stopped shell and then restart ...</span>
$ <b>./mymysh</b>
mymysh$ <span style="color:red;font-weight:bold">!28</span>
cat xyz
Running /bin/cat ...
--------------------
total 76
-rw-r--r-- 1 jas jas   303 Sep 16 21:59 Makefile
-rw-r--r-- 1 jas jas  3310 Sep 16 21:01 history.c
-rw-r--r-- 1 jas jas   377 Sep 16 21:02 history.h
-rw-r--r-- 1 jas jas  2640 Sep 16 22:41 history.o
-rwxr-xr-x 1 jas jas 17216 Sep 16 22:41 mymysh
-rw-r--r-- 1 jas jas 11242 Sep 16 22:36 mymysh.c
-rw-r--r-- 1 jas jas  7792 Sep 16 22:41 mymysh.o
-rw-r--r-- 1 jas jas 12734 Sep 16 20:54 mymysh1.c
-rw-r--r-- 1 jas jas   301 Sep 16 22:40 xxx
-rw-r--r-- 1 jas jas     0 Sep 16 22:48 xyz
--------------------
Returns 0
mymysh$ <span style="color:red;font-weight:bold">pwd</span>
/some/path/or/other/cs1521/ass/ass2
mymysh$ <span style="color:red;font-weight:bold">cd ..</span>
/some/path/or/other/cs1521/ass
mymysh$ <span style="color:red;font-weight:bold">pwd</span>
/some/path/or/other/cs1521/ass
mymysh$ <span style="color:red;font-weight:bold">!!</span>
pwd
/some/path/or/other/cs1521/ass
mymysh$ <span class="comment">control-D</span>
$
</pre>
<p>
To resolve any ambiguities in the above, and to give you a basis
for testing, an executable for the shell is available as
</p>
<pre>$ <b>~cs1521/bin/mymysh</b>
</pre>
<p>
Please let me know asap if you think that there are bugs in the
sample executable.
Disagreeing with one of the above design choices does not
constitute a "bug".
However, if the sample executable behaves differently to what
is stated above, then that is definitely a bug.
</p>

<h3>Challenge</h3>
<p>(Worth kudos, but no marks)</p>
<p>
Implement command pipelines (i.e.
 <i>Cmd<sub>1</sub></i> <b>|</b> <i>Cmd<sub>2</sub></i> <b>|</b> ... <b>|</b> <i>Cmd<sub>n</sub></i>,
where <i>n</i> &#8805;2).
</p>

<p>
Have fun, &nbsp; <i>jas</i>
</p>
</body></html>
