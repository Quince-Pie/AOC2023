use std::collections::HashSet;

const INPUT: &str = include_str!("../input");

fn main() {
    // PART 1:
    println!(
        "{}",
        INPUT
            .lines()
            .map(|l| {
                let colon = l.find(':').unwrap();
                let splitc = l[colon + 1..].trim();
                let split_pipe = splitc
                    .split(['|'])
                    .map(|s| {
                        s.split_whitespace()
                            .map(|num| num.trim().parse::<u32>().unwrap())
                    })
                    .flatten()
                    .collect::<Vec<u32>>();
                let full_len = split_pipe.len();
                let deduped: HashSet<u32> = HashSet::from_iter(split_pipe);
                let sub = full_len - deduped.len();
                if sub == 0 {
                    0
                } else if sub == 1 {
                    1
                } else {
                    1 << sub - 1
                }
            })
            .sum::<u32>()
    );
    // PART 2:
    let mut copies = vec![1; INPUT.lines().count()];
    println!(
        "{}",
        INPUT
            .lines()
            .enumerate()
            .map(|(idx, l)| {
                let colon = l.find(':').unwrap();
                let splitc = l[colon + 1..].trim();
                let split_pipe = splitc
                    .split(['|'])
                    .map(|s| {
                        s.split_whitespace()
                            .map(|num| num.trim().parse::<u32>().unwrap())
                    })
                    .flatten()
                    .collect::<Vec<u32>>();
                let full_len = split_pipe.len();
                let deduped: HashSet<u32> = HashSet::from_iter(split_pipe);
                let matches = full_len - deduped.len();
                if matches > 0 {
                    for next_card in idx + 1..std::cmp::min(idx + 1 + matches, copies.len()) {
                        copies[next_card] += copies[idx];
                    }
                }
                copies[idx]
            })
            .sum::<usize>()
    );
}
