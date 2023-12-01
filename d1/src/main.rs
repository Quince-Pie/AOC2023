const INPUT1: &'static str = include_str!("input");

const NUMSTRS: [(&'static str, u32); 9] = [
    ("one", 1),
    ("two", 2),
    ("three", 3),
    ("four", 4),
    ("five", 5),
    ("six", 6),
    ("seven", 7),
    ("eight", 8),
    ("nine", 9),
];

fn match_numstr(numstr: &str) -> (Option<(usize, u32)>, Option<(usize, u32)>) {
    let fm = NUMSTRS
        .iter()
        .filter_map(|ns| {
            if let Some(x) = numstr.find(ns.0) {
                let r = numstr.rfind(ns.0);
                Some([(x, ns.1), (r.unwrap(), ns.1)])
            } else {
                None
            }
        })
        .flatten();
    let first = fm.clone().min_by_key(|x| x.0);
    let last = fm.max_by_key(|x| x.0);
    (first, last)
}

fn main() {
    println!(
        "{}",
        INPUT1
            .lines()
            .map(|line| {
                let mut fm = line.trim().chars().filter(|ch| ch.is_digit(10));
                let first = fm.next().unwrap_or('0');
                let last = fm.last().unwrap_or(first);
                let num = format!("{}{}", first, last);
                num.parse::<usize>().unwrap()
            })
            .sum::<usize>()
    );

    println!(
        "{}",
        INPUT1
            .lines()
            .map(|line| {
                let numstr = match_numstr(line);
                let mut fm = line.trim().chars().enumerate().filter_map(|(idx, ch)| {
                    if let Some(num) = ch.to_digit(10) {
                        Some((idx, num))
                    } else {
                        None
                    }
                });

                let first_num = fm.next();
                let last_num = fm.last().or(first_num);
                let nums = (first_num, last_num);
                let min = match (nums.0, numstr.0) {
                    (None, None) => 0,
                    (None, Some(e)) => e.1,
                    (Some(e), None) => e.1,
                    (Some(e1), Some(e2)) => {
                        if e1.0 > e2.0 {
                            e2.1
                        } else {
                            e1.1
                        }
                    }
                };
                let max = match (nums.1, numstr.1) {
                    (None, None) => min,
                    (None, Some(e)) => e.1,
                    (Some(e), None) => e.1,
                    (Some(e1), Some(e2)) => {
                        if e1.0 > e2.0 {
                            e1.1
                        } else {
                            e2.1
                        }
                    }
                };
                format!("{min}{max}").parse::<usize>().unwrap()
            })
            .sum::<usize>()
    );
}
