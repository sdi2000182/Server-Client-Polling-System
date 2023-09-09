#!/bin/bash

if [ $# -ne 1 ]; then
  echo "Paradigma: ./tallyVotes.sh tallyResultsFile"
  exit 1
fi

tally_results_file="$1"


input_file="inputFile.txt"

if [ ! -f "$input_file" ]; then
  echo "Error: The input file does not exist"
  exit 1
fi

if [ ! -r "$input_file" ]; then
  echo "Error: Cannot read the input file"
  exit 1
fi

declare -A party_votes
declare -A voted_participants

while read -r line; do
  participant=$(echo "$line" | awk '{$NF=""; print $0}')
  party=$(echo "$line" | awk '{print $NF}')

  if [ "${voted_participants["$participant"]}" ]; then
    echo "Error: $participant has already voted"
    continue
  fi

  voted_participants["$participant"]=1

  ((party_votes["$party"]++))
done < "$input_file"

> "$tally_results_file"

total_votes=0

for party in "${!party_votes[@]}"; do
  total_votes=$((total_votes + party_votes[$party]))
done


sorted_parties=($(
  for party in "${!party_votes[@]}"; do
    echo "$party"
  done | sort
))

for party in "${sorted_parties[@]}"; do
  echo "$party ${party_votes[$party]}" >> "$tally_results_file"
done


echo "Total votes casted: $total_votes" >> "$tally_results_file"

echo "Exoun graftei sto $tally_results_file."
