% projekt: FLP projekt 2: logický projekt: Prolog (VUT FIT)
% autor: Monika Rosinská, xrosin00
% datum: 26. 4. 2021

:- dynamic edge/2, node/1.


%------------INPUT LOADING/SAVING------------
% this part is taken from Martin Hyrs

% detects end if line and end of file
isEOFEOL(C) :-
	C == end_of_file;
	(char_code(C,Code), Code==10).

% reads one line from stdin
read_line(L,C) :-
	get_char(C),
	(isEOFEOL(C), L = [], !;
		read_line(LL,_),
		[C|LL] = L).

% reads every line from std and saves nodes and edges
% input is in format:
%   A B
%   C D
% where one line represents edge, and A, B, C and D are nodes
read_lines :-
    read_line(L,C),
    C \= end_of_file,
    [A, _, B] = L,
    assertz(node(A)),
    assertz(node(B)),
    assertz(edge(A, B)),
    read_lines.

read_lines.


%------------PRINT OUTPUT------------

% prints out given "string" in format A-B C-D E-F

% handles printig in desired format
print_line([X, Y| [] ]) :- format("~w-~w~n", [X, Y]).
print_line([X, Y | T]) :- format("~w-~w ", [X, Y]), print_line([Y|T]).

% handles diving lines
print_paths([]) :- !.
print_paths([X|Output]) :- print_line(X), print_paths(Output).


%------------HELP FUNCTIONS------------

% get all known nodes
get_nodes(N) :- setof(Node, node(Node), N).

% return true if first argument is subset of second argument, returns false otherwise
is_subset([], _).
is_subset([H|T], Y) :- member(H, Y), select(H, Y, Z), is_subset(T,Z). % if H is in set Y, remove H from Y (Z is Y set without H element), and test rest of sets

% returns true if sets are equal, false otherwise
% sets X and Y are equal if X is subset of Y and Y is subset of X
sets_equal(X, Y) :- is_subset(X, Y), is_subset(Y,X).

% returns true, if theres edge between two nodes
% we can move in both ways by edge, so one edge between nodes in any direction is enough
nodes_connected(X, Y) :- edge(X, Y); edge(Y, X).


%------------FIDING HAMILTON CYCLE------------

% finds path between two given nodes
% first line handles saving path (it takes end node, and append visited list); if nodes X and Y are not connected, it  continues at second line
% second line try to find way X to Y by visiting other nodes
% we want to visit each node only once, so we control, that new node is not in visited (and it's not final node)
% if the second node would be only one remaining, it will be added in first line (creates path), and it will "fail" at second line, because we won't find any node Z fitting our needs 
% (it either doens't exist, it's the second node or it's in visited), so find_path ends
find_path(X, Y, Visited, [Y|Visited]) :- nodes_connected(X, Y).
find_path(X, Y, Visited, Path) :- nodes_connected(X, Z), \+ member(Z, Visited), Z \= Y, find_path(Z, Y, [Z|Visited], Path).

% get path between two nodes
% output has to be reversed, to represent path in direction X->Y
get_path(X, Y, Path) :- find_path(X, Y, [X], Output), reverse(Output, Path).

% get hamilton cycle from node
% finds path between node itself, and checks, if it's really hamiltons cycle - path visited every node
% path visited every node, if it contains every node (Nodes is set of all nodes)
% path will have format S N1 N2 ... Ni S, so we have to remove one occurency of start node (S) to check that path contains every node (otherwise path wouldn't be subset of Nodes)
get_hamilton_cycle(Start_node, [Start|Path]) :- get_path(Start_node, Start_node, [Start|Path]), get_nodes(Nodes), sets_equal(Path, Nodes).

% get all existing hamilton cycles in graph and print them
get_all_cycles :- findall(Cycle, get_hamilton_cycle(_, Cycle), Output), print_paths(Output).


%------------RUN PORJECT------------

main :-
    prompt(_, ''),
    read_lines,
    get_all_cycles,
    halt.