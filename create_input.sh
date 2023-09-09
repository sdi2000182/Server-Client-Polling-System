#!/bin/bash


if [ $# -ne 2 ]; then
  echo "Paradeigma: ./create_input.sh politicalParties.txt numLines"
  exit 1
fi

political_parties_file="$1"
num_lines="$2"

if [ ! -f "$political_parties_file" ]; then
  echo "Political parties file '$political_parties_file' not found."
  exit 1
fi


mapfile -t political_parties < "$political_parties_file"


input_file="inputFile.txt"


> "$input_file"


for ((i=0; i<num_lines; i++)); do
  first_name=$(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w "$(shuf -i 3-12 -n 1)" | head -n 1)
  last_name=$(cat /dev/urandom | tr -dc 'a-zA-Z' | fold -w "$(shuf -i 3-12 -n 1)" | head -n 1)
  party=${political_parties[$((RANDOM % ${#political_parties[@]}))]}

  echo "$first_name $last_name $party" >> "$input_file"
done

echo "Dimiourgisa $input_file me $num_lines grammes."
