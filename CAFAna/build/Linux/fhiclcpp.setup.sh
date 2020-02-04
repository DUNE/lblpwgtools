#!/bin/bash

### Adapted from https://unix.stackexchange.com/questions/4965/keep-duplicates-out-of-path-on-source
function add_to_PATH () {
  for d; do

    d=$(cd -- "$d" && { pwd -P || pwd; }) 2>/dev/null  # canonicalize symbolic links
    if [ -z "$d" ]; then continue; fi  # skip nonexistent directory

    if [ "$d" "==" "/usr/bin" ] || [ "$d" "==" "/usr/bin64" ] || [ "$d" "==" "/usr/local/bin" ] || [ "$d" "==" "/usr/local/bin64" ]; then
      case ":$PATH:" in
        *":$d:"*) :;;
        *) export PATH=$PATH:$d;;
      esac
    else
      case ":$PATH:" in
        *":$d:"*) :;;
        *) export PATH=$d:$PATH;;
      esac
    fi
  done
}

function add_to_FHICL_FILE_PATH () {
  for d; do
    d=$(cd -- "$d" && { pwd -P || pwd; }) 2>/dev/null  # canonicalize symbolic links
    if [ -z "$d" ]; then continue; fi  # skip nonexistent directory
    case ":$FHICL_FILE_PATH:" in
      *":$d:"*) :;;
      *) export FHICL_FILE_PATH=$d:$FHICL_FILE_PATH;;
    esac
  done
}

add_to_PATH "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Linux/bin"

if [ "FALSE" ]; then
  add_to_FHICL_FILE_PATH "/dune/app/users/chasnip/CH_DUNE_PRISM/lblpwgtools/CAFAna/build/Linux/fcl"
fi
