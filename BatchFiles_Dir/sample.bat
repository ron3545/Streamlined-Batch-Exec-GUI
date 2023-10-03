@echo off
wmic os get caption /value
wmic path win32_videocontroller get description /value