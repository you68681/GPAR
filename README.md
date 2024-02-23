# Generalized-Planning-for-the-Abstraction-and-Reasoning-Corpus
This code computes planning programs that solve Abstraction and Reasoning Corpus, which builds on top of the [Progressive Generalized Planning](https://github.com/aig-upf/pgp-landmarks) engine. This brach will be merged into Progressive Generalized Planning, so please keep latest update.
It starts with a short tutorial, and later explains every step in more detail.

## Documentation Introduction

ARC-PDDL: Python code for PDDL genertation for the object-centric ARC subset.

ARC-Planner: C++ code for program synthesis implemented by generalized planning.


## Short Tutorial

1. Generate the PDDL for each object-centric ARC subsets.

 python main.py --problem ARC_PROBLEM_FIle --out_dir OUTPUT_FILE --type OBJECT_CENTRIC_TYPE

For exmaple:

```shell
 python main.py --problem  ./ARC-master/data/training/ --out_dir new_color_task --type recolor.txt
```

2. Progran Synthesis

2.1 compile the planner.
```shell
./scripts/compile_main.sh
./scripts/compile_validator.sh
```

2.2. Search a program for ARC tasks 
 ./main.bin GP_Panner Number_of_Probelm_Line Novelty_Threshold Pointer_Indices Problem_Training_FILE Problem_Testing_File Heuristics

 For exmaple:

```shell
./main.bin PGP 3 1 no1:node ./new_color_task/67385a82/cc4_training/ ./new_color_task/67385a82/cc4_testing/ hp hln
```

Heuristics can be none so default heuristic will be used (hp hln). 

The resulting program should looks like:
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
4. Validate the output program, with and without infinite detection.

 ./validator.bin Problem_Testing_File Pointer_Indices

For example:

```shell
./validator.bin ./experiments/67385a82_cc4_training_no1_3_1_hp_hln.prog ./new_color_task/67385a82/cc4_testing/ no1:node
```
Both validations should finish with an `[INFO] GOAL ACHIEVED!`
Great! You have computed your first planning program :-).

