#!/bin/bash

echo "I will try to send files to cluster \" fep.grid.pub.ro \""
scp *.c *.h lazy_test.sh output* input* big_input big_output Makefile run.sh florin_ionel.bardas@fep.grid.pub.ro:/export/home/acs/stud/f/florin_ionel.bardas/APD/Tema1/
echo "Done !"
