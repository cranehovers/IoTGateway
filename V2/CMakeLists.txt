# This file defines Gateway project building logic

PROJECT(gwsp_app)
cmake_minimum_required(VERSION 2.8)

#add into all sub directory
ADD_SUBDIRECTORY(toolkit)
ADD_SUBDIRECTORY(runtime)
ADD_SUBDIRECTORY(server)

