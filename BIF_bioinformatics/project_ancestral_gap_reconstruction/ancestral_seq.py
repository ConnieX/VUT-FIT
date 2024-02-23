# BIF project: Gap reconstruction in ancestral sequences
# Author: Monika Rosinská, xrosin00
# Date: 21. 4. 2022
# VUT FIT

from Bio import Phylo
from Bio import SeqIO


# get id of most probable amino-acid for this line (column)
# input_line - line containing one line from file containing probabilities of amino-acids
# returns tuple: id of amino-acid and id of column
def most_probable_amino_acid(input_line):
    probabilities = input_line.split(',')

    column_id = probabilities[1]

    probabilities = probabilities[2:]  # remove index of node and index of column (not needed here)

    max_prob = 0
    current_id = 0
    max_id = 0

    # probability contains probability of certain amino acid
    for probability in probabilities:
        probability = probability.strip()  # remove new line at the end (it's only in the last element of probabilities)
        # if element has some value (not '-') and
        # if it's probability is higher than current highest probability, save it
        if probability != '-' and float(probability) > float(max_prob):
            max_prob = probability
            max_id = current_id
        current_id = current_id + 1  # iteration
    return max_id + 2, column_id  # + 2 because we removed first two columns at beginning


# get lists of aminoacids from input
# ancestrals - content of file containing probabilities of amino-acids, csv format
def process_lines(ancestrals):
    outputs = {}  # dictionary for ancestrals sequences
    last_node_id = -1
    headers = (ancestrals.__next__()).split(',')  # get headers and "remove" this line from ancestrals
    headers[-1] = headers[-1].strip()  # remove new line from last element

    for line in ancestrals:
        node_id = line.split(',')[0]
        amino_acid_id, column = most_probable_amino_acid(line)  # get the most probable amino acid
        # if current node is different from previous, create index for it and save it as last node
        if node_id != last_node_id:
            outputs[node_id] = []
            last_node_id = node_id
        outputs[node_id].append(headers[amino_acid_id] + column)  # save code of amino-acid and index of column

    return outputs


# sort elements of each line by index of column
def sort_ancestral_sequences(sequences):
    result = {}
    for sequence_id in sequences:
        ancestral_string = ""
        # has format 'aC', where 'a' is one-letter-coding of amino-acid and rest ('C') is index of column
        sequences[sequence_id].sort(key=lambda x: int(x[1:]))

        # get rid of indexes of columns
        # creates string of amino-acids
        for element in sequences[sequence_id]:
            ancestral_string = ancestral_string + element[0]

        # add it to result
        result[sequence_id] = []
        result[sequence_id].append(ancestral_string)
    return result


# insert spaces to given sequences using sequences of leaf nodes
# using logic that nodes, which are more distanced from root have bigger influence if gap is inserted or not
# save result sequences of inner nodes to files
# recursive - will be processed whole tree, for each inner node
# node - root of tree we want to process
# sequences - dictionary of amino-acid sequences of leaf nodes (sequences of living organisms)
# current_lines - dictionary of amino-acid sequences of inner nodes (containing most probable amino-acids on positions, without spaces)
def insert_spaces(node, sequences, current_lines):
    # if node is leaf node, there's nothing to process
    if node.is_terminal():
        return

    output = current_lines.__getitem__(str(node.confidence))[0]  # get sequence of this node
    len_of_seq = len(output)

    results_gap = {}  # dictionary for saving "votes" for inserting gap in positions
    results_no_gap = {}  # dictionary for saving "votes" for inserting gap in positions
    leafs = node.get_terminals()

    # initialization of dictionaries
    for i in range(len_of_seq):
        results_gap[i] = 0.0
        results_no_gap[i] = 0.0

    # get vote for inserting gap on individual positions from each leaf
    for leaf in leafs:
        distance = node.distance(leaf)  # get distance between root and leaf - determines how big influence vote has

        # get "opinion" of leaf on gap for each position in sequence
        for i in range(len_of_seq):
            # if there's gap in current position, vote for gap (add distance, which represents weight of vote)
            # else vote for no gap
            if sequences[leaf.name][i] == '-':
                results_gap[i] = float(results_gap[i]) + distance
            else:
                results_no_gap[i] = float(results_no_gap[i]) + distance

    # for each position compare votes
    # if gap wins (has bigger value), insert gap on this position, otherwise preserve amino-acid code (do nothing)
    for i in range(len_of_seq):
        if float(results_gap[i]) > float(results_no_gap[i]):
            new_output = output[:i] + '-' + output[i+1:]
            output = new_output

    # save string to file
    file_name = "./results/node_" + str(node.confidence) + ".fas"
    file = open(file_name, "w")
    file.write(">" + str(node.confidence) + "\n")
    file.write(str(output))
    file.close()

    # call again for sons (to get their sequences)
    insert_spaces(node.clades[0], sequences, current_lines)
    insert_spaces(node.clades[1], sequences, current_lines)
    return


# get dictionary of sequences from file
# file - content of .fasta file
def process_fasta(file):
    multi_alignment = {}

    for line in file:
        id, sequence = line.id, str(line.seq)
        multi_alignment[id] = sequence

    return multi_alignment


if __name__ == '__main__':
    # load files and process them
    multi_alignment = process_fasta(SeqIO.parse(open('./msa.fasta'), 'fasta'))
    tree = Phylo.read('tree.tre', 'newick')
    ancestral_sequences = process_lines(open('ancestrals.csv'))  # get ancestral sequences from file
    ancestral_sequences = sort_ancestral_sequences(ancestral_sequences)  # sort amino-acids in lines by index of column

    insert_spaces(tree.root, multi_alignment, ancestral_sequences)  # get sequences of amino-acid and gaps for each ancestral node
