#!/bin/bash
/usr/bin//valgrind --leak-check=full ./dist/Debug/GNU-Linux/shopping1_s < ./tests/validation/ECommerce_2019_Q4_200.keyboard 1> /dev/null 2>> ./tests/output/ECommerce_2019_Q4_200.mleaks.full
