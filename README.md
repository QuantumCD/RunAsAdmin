RunAsAdmin
==========

Simple win32 application that will start most executables with administrator rights (UAC prompt). This application will <b>not</b> bypass safeguards in-place by the OS.

Configuration File
==========

The configuration file <b>has to be named <i>RunAsAdmin.cfg</i>!</b> If a file named this isn't placed directly next to the 
executable, then the application won't be able to load any configuration and will fail. When there is no file present, RunAsAdmin will create a default configuration file and alert you to the creation. Be sure to edit this though!

Configuration is extremely easy. It's just a two-line text file (not .txt, .cfg!). The first line is the executable you 
want to launch, and the second line is all the default command line arguments you may need. 

One thing to note is that the command line arguments in the configuration file are the <b>default</b>, meaning if you pass command line arguments to the RunAsAdmin.exe, these will be used instead. This was added via a request, so the launcher would support shell extensions (i.e. dynamic file paths).

Example Configuration
==========
<pre>
PatchIt.exe
--test
</pre>

This will launch PatchIt.exe on start, and also provide the executable with the <i>--test</i> argument if no dynamic arguments are passed. 

The example below, however, will pass <b>just</b> <i>--help</i>. The default parameters are unused unless <b>no</b> dynamic arguments are passed to this application.

<pre>
RunAsAdmin.exe --help
</pre>

How It Works
==========
The application uses a <i>ShellExecute</i> call with the <i>runas</i> verb. Internally, this triggers a UAC prompt which allows your application to gain administrator rights from the UAC without the need of special manifest files that aren't available for all executables.
