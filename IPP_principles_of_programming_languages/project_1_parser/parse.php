<?php

class arguments{
    public $type;
    public $value;
}

class instructions{
    public $opcode;
    public $arg_count;
    public $args = array();
}

class output_code{
    public $instructions = array();
}

//name of istruction and count and type of arguments of inst.
$grammar = array(
    "MOVE" => ["var", "symbol"],
    "CREATEFRAME" => [],
    "PUSHFRAME" => [],
    "POPFRAME" => [],
    "DEFVAR" => ["var"],
    "CALL" => ["label"],
    "RETURN" => [],
    "PUSHS" => ["symbol"],
    "POPS" => ["var"],
    "ADD" => ["var", "symbol", "symbol"],
    "SUB" => ["var", "symbol", "symbol"],
    "MUL" => ["var", "symbol", "symbol"],
    "IDIV" => ["var", "symbol", "symbol"],
    "LT" => ["var", "symbol", "symbol"],
    "GT" => ["var", "symbol", "symbol"],
    "EQ" => ["var", "symbol", "symbol"],
    "AND" => ["var", "symbol", "symbol"],
    "OR" => ["var", "symbol", "symbol"],
    "NOT" => ["var", "symbol"],
    "INT2CHAR" => ["var", "symbol"],
    "STRI2INT" => ["var", "symbol", "symbol"],
    "READ" => ["var", "type"],
    "WRITE" => ["symbol"],
    "CONCAT" => ["var", "symbol", "symbol"],
    "STRLEN" => ["var", "symbol"],
    "GETCHAR" => ["var", "symbol", "symbol"],
    "SETCHAR" => ["var", "symbol", "symbol"],
    "TYPE" => ["var", "symbol"],
    "LABEL" => ["label"],
    "JUMP" => ["label"],
    "JUMPIFEQ" => ["label", "symbol", "symbol"],
    "JUMPIFNEQ" => ["label", "symbol", "symbol"],
    "EXIT" => ["symbol"],
    "DPRINT" => ["symbol"],
    "BREAK" => [],
);

//names of instructions - for easiest controlling validity of instruction in code
$dictionary = array("MOVE", "CREATEFRAME", "PUSHFRAME", "POPFRAME", "DEFVAR", "CALL", "RETURN",
    "PUSHS", "POPS", "ADD", "SUB", "MUL", "IDIV", "LT", "GT", "EQ", "AND", "OR", "NOT", "INT2CHAR", "STRI2INT",
    "READ", "WRITE", "CONCAT", "STRLEN", "GETCHAR", "SETCHAR", "TYPE", "LABEL", "JUMP", "JUMPIFEQ", "JUMPIFNEQ", "EXIT",
    "DPRINT", "BREAK");

//function for creating XML output
//$inst_list - list of loaded instructions
function writeXML(output_code $inst_list){
    $xml = new XMLWriter();
    $xml->openMemory();
    $xml->setIndent(true);
    $xml->setIndentString("    ");
    $xml->startDocument('1.0', 'utf-8');
    $xml->startElement("program");
    $xml->writeAttribute("language", "IPPcode20");
    //go through all instructions in list
    for($i = 0; $i < count($inst_list->instructions); $i++){
        $xml->startElement("instruction");
        $xml->writeAttribute("order", ($i+1)); //i+1 because it's indexed from 0 in array but in output it's indexed from 1
        $xml->writeAttribute("opcode", $inst_list->instructions[$i]->opcode);
        //go through all arguments of the instruction
        for($j = 0; $j < $inst_list->instructions[$i]->arg_count; $j++) {
            $xml->startElement("arg".($j+1)); //j+1 because it's indexed from 0 in array but in output it's indexed from 1
            $xml->writeAttribute("type", $inst_list->instructions[$i]->args[$j]->type);
            $xml->text($inst_list->instructions[$i]->args[$j]->value);
            $xml->endElement();
        }
        $xml->endElement();
    }
    $xml->endElement();
    $xml->endDocument();
    echo $xml->outputMemory();
}

//function for controlling arguments
//$instruct - instruction object (filled)
function control_args(instructions $instruct){
    global $grammar;
    $params = $grammar[$instruct->opcode];
    for($i = 0; $i < $instruct->arg_count; $i++){
        //if arg type in grammar is symbol, argument must be bool/int/string/nil/var
        if($params[$i] == "symbol"){
            if(!($instruct->args[$i]->type == "bool" || $instruct->args[$i]->type == "int" || $instruct->args[$i]->type == "string" || $instruct->args[$i]->type == "nil" || $instruct->args[$i]->type == "var")){
                exit(23);
            }
        }
        //else it has to match name of type from grammar
        elseif($params[$i] != $instruct->args[$i]->type){
            exit(23);
        }
    }

}

//create new argument
//$first_part - first part of gotten argument = type of argument
//$second_part - second part of gotten argument = value of argument
//$instruct - instruction object
function new_arg($first_part, $second_part, $instruct){
    $arg = new arguments();
    $arg->type = $first_part;
    $arg->value = $second_part;
    array_push($instruct->args, $arg);
}

//control argument - if type part and value part match (for example - int@45 - is 45 int?)
//$argument - gotten argument (not parsed, in format type@value
//$inst - instruction object
function arg_type($argument, $inst){
    //if value of $argument is notargument-1, it doesnt exist; this shouldn't happen
    if($argument == "notargument-1"){
        return "";
    }
    //split argument by first @
    $splitted_arg = preg_split("/@/", $argument, 2);

    //type -> instruction must be "READ" and type must be string/int/bool and there can't be any @ (=> line wasn't splitted)
    if($inst->opcode == "READ" && ($splitted_arg[0] == "string" || $splitted_arg[0] == "int" || $splitted_arg[0] == "bool") && count($splitted_arg) == 1){
        new_arg("type", $splitted_arg[0], $inst);
    }
    //integer
    elseif($splitted_arg[0] == "int"){
        if(preg_match("/^(-|\+)?[0-9]+$/", $splitted_arg[1])){
            new_arg($splitted_arg[0], $splitted_arg[1], $inst);
        }
        else{
            exit(23);
        }
    }
    //string
    elseif($splitted_arg[0] == "string"){
        #regex - escape sequences; cannot be "\", except \+three numbers
        if(preg_match("/^([^\\\\ \r\n\t\f\v]|(\\\\[0-9]{3}))*$/", $splitted_arg[1])){
            new_arg($splitted_arg[0], $splitted_arg[1], $inst);
        }
        else{
            exit(23);
        }
    }
    //variable
    elseif($splitted_arg[0] == "GF" || $splitted_arg[0] == "LF" ||$splitted_arg[0] == "TF" ){
        #regex - escape sequences
        if(preg_match("/^[ěščřžýáíéóúůďťňĎŇŤŠČŘŽÝÁÍÉÚŮa-zA-Z_\-$&%*!?]+[ěščřžýáíéóúůďťňĎŇŤŠČŘŽÝÁÍÉÚŮa-zA-Z0-9_\-$&%*!?\S]*/", $splitted_arg[1])){
            $arg = new arguments();
            $arg->type = "var";
            $arg->value = $argument;
            array_push($inst->args, $arg);
        }
        else{
            exit(23);
        }
    }
    //bool
    elseif($splitted_arg[0] == "bool"){
        if($splitted_arg[1] == "true" || $splitted_arg[1] == "false"){
            new_arg($splitted_arg[0], $splitted_arg[1], $inst);
        }
        else{
            exit(23);
        }
    }
    //nil
    elseif($splitted_arg[0] == "nil"){
        if($splitted_arg[1] == "nil"){
            new_arg($splitted_arg[0], $splitted_arg[1], $inst);
        }
        else{
            exit(23);
        }
    }
    //label
    elseif (count($splitted_arg) == 1){
        //regex - letters (czech too), numbers, some special characters; cannot begin by number
        if(preg_match("/^[ěščřžýáíéóúůďťňĎŇŤŠČŘŽÝÁÍÉÚŮa-zA-Z_\-$&%*!?]+[ěščřžýáíéóúůďťňĎŇŤŠČŘŽÝÁÍÉÚŮa-zA-Z0-9\S]*/", $splitted_arg[0])){
            $arg = new arguments();
            $arg->type = "label";
            $arg->value = $argument;
            array_push($inst->args, $arg);
        }
        else{
            exit(23);
        }
    }
    else{
        exit(23);
    }
}

//parse arguments
//$output = object for output
//$arg1, $arg2, $arg3 - optional argument's; if no argument is given, their value is "notargument-1"
function parse_args($output, $instruction, $number_of_args, $arg1 = "notargument-1", $arg2 = "notargument-1", $arg3 = "notargument-1"){
    $inst = new instructions();
    $inst->opcode = $instruction;
    $inst->arg_count = $number_of_args;
    array_push($output->instructions, $inst);
    //control all arguments
    if($number_of_args > 0){
        arg_type($arg1, $inst);
    }
    if($number_of_args > 1){
        arg_type($arg2, $inst);
    }
    if($number_of_args > 2){
        arg_type($arg3, $inst);
    }
    control_args($inst);
}

$stats_found = 0; //variable for checking if combination of arguments is valid
$stat_pos = 0; //position of --stat=file argument
$loc = 0; //var for counting instructions
$comments = 0; //var for counting comments
$label = 0; //var for counting labels
$jumps = 0; //var for counting jumps (conditional and unconditional jumps, calls, returns)
$outputfile = null;

//check only if there are arguments
if($argc > 1){
    //if any of arguments is "--help", print help and exit with code 0
    for($i = 1; $i < $argc; $i++){
        if($argv[$i] == "--help") {
            echo "parse.php - parser of language IPPcode20\nWrite your code to STDIN, otput is XML format of your code on STDOUT.\n";
            echo "\nARGUMENTS:\n--stats=file ... print statistic to output file (can create file); compulsory argument in case of use ones or more of next arguments:\n";
            echo "  --loc ... prints count of lines with instructions\n";
            echo "  --comments ... prints count of comments\n";
            echo "  --labels ... prints count of labels\n";
            echo "  --jumps ... print count of conditional and unconditional jumps, calls and returns\n";
            exit(0);
        }
    }
    //if there's another argument than known arguments, exist with code 10 (wrong argument)
    for($i = 1; $i < $argc; $i++){
        if($argv[$i] != "--loc" && $argv[$i] != "--comments" && $argv[$i] != "--jumps" && $argv[$i] != "--labels" && preg_split("/=/", $argv[$i])[0] != "--stats") {
            echo "Wrong argument was given.\n";
            exit(10);
        }
        //count number of occurrences of argument "--stats=file"
        if(preg_split("/=/", $argv[$i])[0] == "--stats"){
            $stat_pos = $i; //remember position of --stats
            $stats_found += 1;
        }
    }
    //if argument "--stats=file" is there 0 times or more than 1 time -> error, exit with code 10 (wrong combination of arguments)
    if($stats_found != 1){
        echo "Argument --stats=file missing.\n";
        exit(10);
    }
    $outputfile = fopen(preg_split("/=/", $argv[$stat_pos])[1], "w"); //create (open) outputfile
}



$input = []; //array for input
$index = 0; //index for saving lines
$file = STDIN; //read from STDIN
$output = new output_code(); //object for output

//read from STDIN
while(($line = fgets($file)) != null){
    $line = ltrim($line, " "); //delete spaces before first character
    //if first character isn't # - it' not comment
    if($line[0] != "#" && $line[0] != "\n" && strlen($line[0]) > 0){
        $input[$index] = $line; //save it to array
        $split = preg_split("/#/", $input[$index]); //remove comments
        if($split[0] != $input[$index]){
            $input[$index] = $split[0];
            $comments++;
        }
        $input[$index] = rtrim($input[$index]); //set new line
        $index++; //move index
    }
    elseif($line[0] == "#"){
        $comments++;
    }

}

//control if first line is valid
if(!preg_match("/^\.IPPCODE20 *(#.*)?$/", strtoupper($input[0]))){
    exit(21);
}

//for each line in input (except first - ippcode20)
for($i = 1; $i < $index; $i++) {
    $split_line = preg_split("/ /", $input[$i], -1, PREG_SPLIT_NO_EMPTY); //split by space - controls if it's not empty line
    if(count($split_line) > 0) {
        $split_line[0] = strtoupper($split_line[0]); //set names to upper (moVE -> MOVE)
        //control if instruction is in dictionary (is valid)
        if (!in_array($split_line[0], $dictionary) && (strlen($split_line[0]) > 0) && $split_line[0] != "\n") {
            exit(22);
        }
        else {
            foreach ($grammar as $key => $word){
                //set expected count of args
                if ($key == $split_line[0]) {
                    if($key == "LABEL"){
                        $label++;
                    }
                    if($key == "CALL" || $key == "JUMP" || $key == "JUMPIFEQ" || $key == "JUMPIFNEQ" || $key == "RETURN"){
                        $jumps++;
                    }
                    $count_of_args = 0;
                    for ($j = 0; $j < count($split_line); $j++) {
                        if ($split_line[$j] != "" && $split_line[$j] != "\n") {
                            $count_of_args++;
                        }
                    }
                    //control if I got right count of arguments
                    if ($count_of_args - 1 == count($word)) {
                        //call function for parsing arg with right number of args
                        if (count($split_line) == 1) {
                            parse_args($output, $split_line[0], 0);
                        } elseif (count($split_line) == 2) {
                            parse_args($output, $split_line[0], $count_of_args-1, $split_line[1]);
                        } elseif (count($split_line) == 3) {
                            parse_args($output, $split_line[0], $count_of_args-1, $split_line[1], $split_line[2]);
                        } elseif (count($split_line) == 4) {
                            parse_args($output, $split_line[0], $count_of_args-1, $split_line[1], $split_line[2], $split_line[3]);
                        }
                    } else {
                        exit(23);
                    }
                }
            }
        }
    }
}
$loc = $index - 1;
writeXML($output);
//if arguments were given, print data to the outputfile
if($argc > 1){
    foreach($argv as $argument){
        if($argument == "--loc"){
            fwrite($outputfile, $loc."\n");
        }
        elseif($argument == "--comments"){
            fwrite($outputfile, $comments."\n");
        }
        elseif($argument == "--labels"){
            fwrite($outputfile, $label."\n");
        }
        elseif($argument == "--jumps"){
            fwrite($outputfile, $jumps."\n");
        }
    }
    fclose($outputfile);
}
fclose($file);