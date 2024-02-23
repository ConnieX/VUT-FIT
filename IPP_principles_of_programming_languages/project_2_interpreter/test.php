<?php

$total_tests = 0; //counter of total number of tests
$ok_tests = 0; //counter for successful tests
$jexml = "/pub/courses/ipp/jexamxml/jexamxml.jar"; //path to jexaml NOTE: I forgot this possible argument and I didn't have time to fix it normal way... If I had more time I wouldn't use global variable for this

//open directory and load files
function open_dir($dir){
    if (is_dir($dir)){
        $files = array_slice(scandir($dir), 2);
    }
    else{
        //if directory doesn't exists (can happen only during opening first dir
        exit(11);
    }
    return $files;
}

//get src files
function src_files($files, $dir){
    $src_files = [];
    foreach ($files as $filename){
        $path = $dir . "/" . $filename;
        //save only .src files
        if (preg_match("/.*\.src/", $filename) & file_exists($path)){
            $filename = explode(".src", $filename)[0];
            array_push($src_files, $filename);
        }
    }
    return $src_files;
}

//gen another dirs from dir
function get_dirs($dir, $files){
    $dirs = [];
    foreach ($files as $filename){
        $path = $dir . "./" . $filename;
        if (is_dir($path)){
            array_push($dirs, $filename);
        }
    }
    return $dirs;
}

//check if output of parser is same as expected
function check_output_parser($directory, $name_of_file, $output){
    //try to open file; else create empty one
    $new = "false";
    if (is_file($directory . "/" . $name_of_file . ".out")){
        $test_out = $directory . "/" . $name_of_file . ".out";
    }
    else{
        $new = "true";
        $test_out = fopen($directory . "/" . $name_of_file . ".out", "w");
        fclose($test_out);
        $test_out = $directory . "/" . $name_of_file . ".out";
    }
    //save output of parser into file
    $output_file = fopen( $name_of_file . ".xml", "w");
    fwrite($output_file, $output);
    fclose($output_file);
    $output_file = $name_of_file . ".xml";

    //proc for controlling output
    $descriptors = array(
        0 => array("pipe", "r"),  // STDIN
        1 => array("pipe", "w"),  // STDOUT
    );
    global $jexml;
    $proc = proc_open('java -jar ' . $jexml . ' ' . $output_file . ' ' . $test_out, $descriptors, $pipes);
    $return = proc_close($proc); //returns 0 if files are same
    //if files matches -> SUCCESSFUL; else FAILED
    if($return == 0){
        echo"                <tr><td><b>$name_of_file</b></td><td><font color=\"green\"><b>SUCCESS</b></font></td></tr></div>\n";
        global $ok_tests;
        $ok_tests++;
    }
    else {
        echo"                <tr><td><b>$name_of_file</b></td><td><font color=\"red\"><b>FAILED</b></font></td><td>Wrong output</td></tr></div>\n";
    }
    //if new .out file was created, delete it
    if ($new == "true"){
        unlink($directory . "/" . $name_of_file . ".out");
    }
    //delete file witch parser output
    unlink($name_of_file . ".xml");
}

//check if output of interpret is same as expected
function check_output_interpret($directory, $name_of_file, $output){
    //try to open .out file; else create empty one
    $new = "false";
    if (is_file($directory . "/" . $name_of_file . ".out")){
        $test_out = $directory . "/" . $name_of_file . ".out";
    }
    else{
        $new = "true";
        $test_out = fopen($directory . "/" . $name_of_file . ".out", "w");
        fclose($test_out);
        $test_out = $directory . "/" . $name_of_file . ".out";
    }
    //save output of interpret to file
    $output_file = fopen("output.txt", "w");
    fwrite($output_file, $output);
    fclose($output_file);
    $output_file = "output.txt";

    system('diff ' . $output_file . ' ' . $test_out, $return);
    //if files matches -> SUCCESSFUL; else FAILED
    if($return == 0){
        echo"                <tr><td><b>$name_of_file</b></td><td><font color=\"green\"><b>SUCCES</b></font></td></tr></div>\n";
        global $ok_tests;
        $ok_tests++;
    }
    else {
        echo"                <tr><td><b>$name_of_file</b></td><td><font color=\"red\"><b>FAILED</b></font></td><td>Wrong output</td></tr></div>\n";
    }
    //if new .out file was created, delete it
    if ($new == "true"){
        unlink($directory . "/" . $name_of_file . ".out");
    }
    //delete file with output of interpret
    unlink("output.txt");
}

//check return value
function check_return_val($directory, $name_of_file, $ret_val, $output_val, $only, $interpret_val){
    //try to open .rc file, if it doesn't exists, act like return value should be 0
    if (is_file($directory . "/" . $name_of_file . ".rc")){
        $test_out = fopen($directory . "/" . $name_of_file . ".rc", "r");
        $value = fread($test_out, 2);
    }
    else{
        $value = 0;
    }
    //if values aren't same, end of function (another file), if values are same and it's not 0, increase ok tests counter, else check output
    if ($ret_val != $value) {
        echo"                <tr><td><b>$name_of_file</b></td><td><font color=\"red\"><b>FAILED</b></font></td><td>expected: &nbsp $value</td><td>recieved: &nbsp $ret_val</td></tr></div>\n";
    }
elseif ($ret_val != 0){
        echo"                <tr><td><b>$name_of_file</b></td><td><font color=\"green\"><b>SUCCES</b></font></td></tr></div>\n";
        global $ok_tests;
        $ok_tests++;
    }
    if($ret_val == 0){
        if ($only == "interpret"){
            check_output_interpret($directory, $name_of_file, $output_val);
        }
        elseif ($only == "parse"){
            check_output_parser($directory, $name_of_file, $output_val);
        }
    }
}

//function for testing
function do_it_all($directory, $tested_file, $recursive, $only, $interpret){
    echo "        <h4>Složka: $directory:</h4>\n";
    echo"            <table width=\"1000\"><col width=\"130\">\n";
    $files = open_dir($directory); //get files
    $src_files = src_files($files, $directory); //get src files
    //if it's intepret only, run only interpret (using python3.8), else run parser (php7.4)
    if ($only == "interpret"){
        $run = "python3.8 ";
    }
    else{
        $run = "php7.4 ";
    }
    //for each .src file
    foreach ($src_files as $file) {
        global $total_tests;
        $total_tests++; //increase counter of count of tests
        //process for running interpret/parser
        $descriptors = array(
            0 => array("pipe", "r"),  // STDIN
            1 => array("pipe", "w"),  // STDOUT
        );
        //if we want to test interpret, try to open .in file, if it doesn't exist, create empty one
        if($only == "interpret"){
            if (is_file($directory . "/" . $file . ".in")) {
                $in_file = fopen($directory . "/" . $file . ".in", "r");
            } else {
                $in_file = fopen($directory . "/" . $file . ".in", "w");
            }
            $proc = proc_open($run . $tested_file ."  --source=" . $directory . "/" . $file . ".src --input=" . $directory . "/" . $file . ".in", $descriptors, $pipes);
            fclose($in_file);
        }
        else{
            $proc = proc_open($run . $tested_file . " <" . $directory . "/" . $file . ".src", $descriptors, $pipes);
        }

        $stdout = stream_get_contents($pipes[1]);
        $return_value  = proc_close($proc);
        //if only parse or if interpret was tested or if parser was tested and return value wasn't 0: control return value
        if ($only == "parse" || $run == "python3.8 " || ($run == "php7.4 " && int($return_value) != 0)){
            check_return_val($directory, $file, $return_value, $stdout, $only, $interpret);
        }
        //else do interpret (we have parser output)
        else{
            //save parser output to file
            $xml_file = fopen($file . ".xml", "w");
            fwrite($xml_file, $stdout);
            fclose($xml_file);
            //proc for running of test
            $descriptors = array(
                0 => array("pipe", "r"),  // STDIN
                1 => array("pipe", "w"),  // STDOUT
            );
            $new = "false";
            //if we want to test interpret, try to open .in file, if it doesn't exist, create empty one
            if (is_file($directory . "/" . $file . ".in")){
                $test_out = $directory . "/" . $file . ".in";
            }
            else{
                $new = "true";
                $test_out = fopen($directory . "/" . $file . ".in", "w");
                fclose($test_out);
                $test_out = $directory . "/" . $file . ".in";
            }
            $proc = proc_open("python3.8 ". $interpret ."  --source=" . $file . ".xml " . "--input=" . $test_out, $descriptors, $pipes);
            $stdout = stream_get_contents($pipes[1]);
            $return_value  = proc_close($proc);
            check_return_val($directory, $file, $return_value, $stdout, "interpret", $interpret);
            //if new .in file was created, delete it
            if ($new == "true"){
                unlink($directory . "/" . $file . ".in");
            }
            //delete file with parser output
            unlink($file . ".xml");
        }
    }
    echo "            </table>\n";

    //if --recursive argument was given, do directories in given dir
    if ($recursive == "true"){
        $dirs = get_dirs($directory, $files);
        foreach ($dirs as $dir){
            do_it_all($directory.$dir, $tested_file, $recursive, $only, $interpret);
        }
    }
}

$directory = "./"; //variable for dir where tests are
$recursive = "false"; //if i should check directories in $directory
$parse_script = "parse.php"; //path and name of parse script
$interpret_script = "interpret.py"; //path and name of interpret
$only = "all"; //if i am controlling parser and interpret or only parser or only interpret

//check given argument
for ($i = 1; $i < count($argv); $i++){
    $arg = explode("=", $argv[$i]);
    switch ($arg[0]){
        case "--directory":
            $directory = $arg[1];
            break;
        case "--recursive":
            $recursive = "true";
            break;
        case "--parse-script":
            $parse_script = $arg[1];
            break;
        case "--int-script":
            $interpret_script = $arg[1];
            break;
        case "--parse-only":
            if ($only == "all"){
                $only = "parse";
            }
            else{
                exit(10);
            }
            break;
        case "--int-only":
            if ($only == "all"){
                $only = "interpret";
            }
            else{
                exit(10);
            }
            break;
        case "--jexamxml":
            global $jexml;
            $jexml = $arg[1];
            break;
        case "--help":
            echo "Help me if you can I'm feeling down\n";
            break;
        default:
            break;
    }
}

//print first part of html
echo "<!DOCTYPE html>
    <html>
      <head>
         <title>IPPcode20</title>
         <meta charset=\"UTF-8\">
         <style type=\"text/css\">
         body {
         	overflow-x: hidden;
         	width: 980px;
         	margin: 0 auto;
         	background-color: blanchedalmond;
         }
         .content {
         	background-color: rgba(255,255,255,0.6);
         	padding: 10px;
         	border-radius: 10px;
         	margin-top: 10px;
         }
         h1 {
         	text-align: center;
         	font-size: 54px;
         }
         .result {
         	text-align: center;
         	font-size: 24px;
         }
         .result span {margin-right: 25px;}
         .result span .green {
         	color: green;
         	margin-right: 5px;
         }
         .result span .red {
         	color: red;
         	margin-right: 5px;
         }
         .dirg, .dirr {
         	border: 2px solid green;
         	padding: 10px;
         	display: inline-block;
         }
         .dirr{
         	border: 2px solid red;
         }
         </style>
      </head>
      <body>
        <div class=\"content\">
         <h1>IPP2020 testy</h1>
        <h3>Testovací sada: $directory</h3>
        <h3>Testováno: $only</h3>
";

//do testing
if($only == "interpret"){
    do_it_all($directory, $interpret_script, $recursive, $only, $interpret_script, $total_tests, $ok_tests);
}
else{
    do_it_all($directory, $parse_script, $recursive, $only, $interpret_script, $total_tests, $ok_tests);
}

$percent = intval(($ok_tests/$total_tests)*100); //successfully of tests in %

//print end of html
echo "
        <h3>Úspěšnost: &nbsp <font color=\"green\"><b>$ok_tests/$total_tests</b></font> &nbsp tj. <font color=\"green\"><b>$percent%</b></font></h3>
        <hr>
    </body>
</html>";

