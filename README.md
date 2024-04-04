# Generalized-Planning-for-the-Abstraction-and-Reasoning-Corpus
This code computes planning programs that solve Abstraction and Reasoning Corpus, which builds on top of the [Progressive Generalized Planning](https://github.com/aig-upf/pgp-landmarks) engine. This brach will be merged into Progressive Generalized Planning, so please stay tuned.

## Code Introduction

ARC-PDDL: Python code for PDDL generation for the object-centric ARC subset.

ARC-Planner: C++ code for program synthesis implemented by generalized planning.


## Short Tutorial



1. Generate the PDDL for each object-centric ARC task.
   
1.1 Python Prerequisites.

```pip install -r requirements.txt```

1.2 Generate the PDDL.

 python main.py --problem ARC_Problem_Folder --out_dir Output_Folder --type Object_Centric_Type

For exmaple:

```shell
 cd ARC-PDDL
```

```shell
 python main.py --problem ./ARC-master/data/training/ --out_dir ../ARC-Planner/new_color_task --type recolor.txt
```

2. Progran Synthesis

2.1 compile the planner for program synthesis and validation.

```shell
 cd ARC-Planner
```

```shell
./scripts/compile_main.sh
./scripts/compile_validator.sh
```

2.2. Search a program for an ARC task.

 ./main.bin GP_Planner Program_Line_Number Novelty_Threshold Pointer_Indices Training_Folder Testing_Folder Heuristics

 (note: Different pointer_indices are separated by commas, such as no1:node,no2:node)
 
 For exmaple:

```shell
./main.bin PGP 3 1 no1:node ./new_color_task/67385a82/cc4_training/ ./new_color_task/67385a82/cc4_testing/ hp hln
```
The output should looks like:
```shell
[INFO] SOLUTION FOUND!!!
[INFO] Expanded nodes: 26
[INFO] Evaluated nodes: 177 (deadends: 123)
[INFO] Evaluated states: 56
0. check-condition(node-size(no1,size-1))
1. goto(3,!(( zf = 1 )))
2. update-color(no1,color-3,color-8)
3. inc(no1)
4. goto(0,!(( zf = 1 )))
5. end
```

Note: ```check-condition``` is the same as the ```test``` instruction mentioned in our paper. They will be merged in the next version.

Heuristics can be none so default heuristics will be used (hp hln). 

The generated results will be saved under ./experiments/ folder. Every experiment has a ".out" file, if it finds a solution also generates a ".prog" file with the planning program.


4. Validate the output program.

 ./validator.bin Program_Solution Testing_Folder Pointer_Indices

For example:

```shell
./validator.bin ./experiments/67385a82_cc4_training_no1_3_1_hp_hln.prog ./new_color_task/67385a82/cc4_testing/ no1:node
```
All validations should finish with an `[INFO] GOAL ACHIEVED!`
Great! You have computed your first planning program :-).

