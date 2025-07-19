# BitTorrent Client Implementation in C

A step-by-step implementation of a BitTorrent client in C, designed as a project-based learning experience.

## Motivation

I love programming, but I've been stuck in "vibe coding hell" lately. AI tools like Claude are incredibly productive for getting things done, but I realized I wasn't learning anything meaningful - just copy-pasting solutions without understanding the fundamentals.

So I decided to flip the script. Instead of using Claude to write code for me, I used it to generate a structured learning project with just enough boilerplate and guidance to get me started. The goal isn't to have Claude solve everything, but to create a framework where I can learn by implementing complex systems from scratch.

I chose BitTorrent because it's a fascinating protocol that touches on networking, file I/O, concurrency, and distributed systems. I picked C because... why not embrace the challenge? No hand-holding, no abstractions - just pure systems programming.

This method is totally replicable with other complex topics: TCP stack implementation, git internals, browser engines, operating system kernels, or any system you want to deeply understand. The key is using AI to create the learning scaffold, not to do the learning for you.

Time to escape vibe coding hell and actually learn something! 🚀

## Overview

This project implements a functional BitTorrent client from scratch in C, breaking down the complex protocol into manageable steps. Each step focuses on a specific component of the BitTorrent protocol and includes comprehensive explanations of both the protocol and the C programming concepts involved.

## Prerequisites

- GCC compiler (or compatible C compiler)
- POSIX-compliant system (Linux/Mac)
- Basic knowledge of C programming

## Setup

1. Clone or download this project
2. Navigate to the project directory
3. Each step is self-contained in its own directory

## Project Structure

```
torrent/
├── README.md
├── step1_bencode_parser/
│   ├── bencode.h
│   ├── bencode.c
│   ├── test_bencode.c
│   └── bencode_explanation.md
└── [additional steps will be added]
```

## Steps

### Step 1: Bencode Parser
**Directory:** `step1_bencode_parser/`
**Objective:** Implement a parser for Bencode format used in .torrent files
**Test:** `cd step1_bencode_parser && gcc -o test_bencode test_bencode.c bencode.c && ./test_bencode`

## Testing

Each step includes a test file that can be compiled and run independently. Follow the test instructions in each step's explanation file.

## Learning Path

1. **Step 1:** Bencode parsing - Foundation for reading .torrent files
2. **Step 2:** Torrent file parsing - Extract metadata from .torrent files
3. **Step 3:** Tracker communication - HTTP requests to BitTorrent trackers
4. **Step 4:** Peer discovery - Find and connect to peers
5. **Step 5:** Peer wire protocol - Handshake and message exchange
6. **Step 6:** Piece management - Download and verify file pieces
7. **Step 7:** File assembly - Reconstruct files from pieces
8. **Step 8:** Complete client - Integration and final testing

## License

This project is for educational purposes.