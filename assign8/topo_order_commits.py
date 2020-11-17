#!/usr/local/cs/bin/python3

import os
import sys
import zlib

cwd = os.getcwd() # turn pwd into string

# Number 1 of HW, finding directory that contains .git
def find_top_level_dir(cwd):
    temp_cwd = cwd # make a copy of cwd global variable
    while temp_cwd != '/': # loop as long as temp_cwd is not root
        dir_list = os.listdir(temp_cwd) # get all files and dirs except . and ..
        for f in dir_list: # loop through the list of files
            if f == '.git': # if .git found
                return temp_cwd # this is the top level dir
        temp_cwd = os.path.dirname(temp_cwd) # not found, chop off last part and move up
    if temp_cwd == '/': # if at root after looping
        sys.stderr.write('Not inside a Git repository\n')
        sys.exit(1) # exit with status 1

# Scrape all branch hashes
def scrape_branch_hash(cwd):
    tld = find_top_level_dir(cwd)
    branch_dir = tld + '/.git/refs/heads' # the directory with all the branches
    branch_list = os.listdir(branch_dir) # find all contents of that directory
    branch_hash = [] 
    for file in branch_list:
        with open(branch_dir + '/' + file) as f: # open each file
            content = f.readlines() # read contents
            str_version = ''.join(content) # turn into string
            str_version = str_version[:-1] # delete \n
            branch_hash.append(str_version) # add to list
    return branch_hash

# Scrape all compressed commit hashes
def scrape_commit_compressed_hash(cwd):
    tld = find_top_level_dir(cwd)
    commit_dir = tld + '/.git/objects' # the directory with all the commit hashes
    commit_hash_first_list = os.listdir(commit_dir) # find all contents of that directory
    commit_hash_first_list.remove('info')
    commit_hash_first_list.remove('pack')
    commit_hash = []
    for file in commit_hash_first_list: # for each two digit directory
        str_version = ''.join(os.listdir(commit_dir + '/' + file)) # each latter half of hash
        commit_hash.append(file + str_version) # add two digit + latter half
    return commit_hash

# Getting full directory to compressed hashes
def full_dir_compressed_commit(cwd): # full dir looking like blahblah/objects/xx/zzzzzzzz
    commit_hash_changed = scrape_commit_compressed_hash(cwd)
    full_dir_list = []
    for f in commit_hash_changed:
        first = f[:2]
        second = f[2:]
        directory = find_top_level_dir(cwd) + '/.git/objects/' + first + '/' + second
        full_dir_list.append(directory)
    return full_dir_list

# Decompressing compressed commit hashes
def decompress_commit_hash(cwd):
    decompressed_hash_list = []
    full_dir_list = full_dir_compressed_commit(cwd)
    for f in full_dir_list:
        compressed = open(f, 'rb').read() # open each commit hash
        decompressed = zlib.decompress(compressed) # turn the gibberish into sth meaningful
        decompressed_str = str(decompressed)
        decompressed_hash_list.append(decompressed_str)
    return decompressed_hash_list

# Going through decompressed commit hashes and finding parent hashes
def delete_but_parent_hash(cwd):
    parent_hash_list = []
    full_hash = decompress_commit_hash(cwd) # get full disgusting list
    for f in full_hash:
        if f.count('parent') == 0: # if it doenst contain word 'parent' fill with empty str
            new_item = ''
            parent_hash_list.append(new_item) # add empty str
        else:
            how_many = f.count('parent') # how many occurences of parent
            list_of_parents = [] # might be multiple parents
            # finding all indices where the word 'parent' starts
            all_indices = [i for i in range(len(f)) if f.startswith('parent', i)]
            for i in all_indices:
                new_item = f[i+7:i+47] # finding all parent hashes
                list_of_parents.append(new_item) # add parent hash to list
            parent_hash_list.append(list_of_parents)
    return parent_hash_list # a list of lists

# Create a dictionary with commit compressed hash as key and parent hash as value
commit_hash = scrape_commit_compressed_hash(cwd)
parent_hash = delete_but_parent_hash(cwd)
commit_parent_dict = {commit_hash[i]: parent_hash[i] for i in range(len(commit_hash))}

# Delete items from dictionary that are not commits
full_hash = decompress_commit_hash(cwd) 
index_del = []
for i in range(len(full_hash)):
    if full_hash[i].count('commit') == 0: # if it doesn't contain 'commit' at all
        index_del.append(i)
    elif full_hash[i].index('commit') != 2: # if it contains 'commit' but wrong position
        index_del.append(i)
keys_to_delete = []
i = 0
for key in commit_parent_dict.keys():
    if i in index_del:
        keys_to_delete.append(key)
    i = i + 1
for key in keys_to_delete:
    if key in commit_parent_dict:
        del commit_parent_dict[key]
        
# Create CommitNode Class
class CommitNode:
    def __init__(self, commit_hash: str):
        self.commit_hash = commit_hash
        self.parents = {}
        self.children = {}
    def add_parent(self, parent_hash: str):
        self.parents.add(parent_hash)

# Turn dictionary into objects
i = 0
for key in commit_parent_dict:
    i = str(i)
    obj = 'a' + i
    obj  = CommitNode(key)
    print(obj.commit_hash)
    i = int(i)
    i = i + 1
        
# Test script
print('The following is a dictionary mapping a commit hash to its parent or parents\' hashes\n')
print(str(commit_parent_dict))
