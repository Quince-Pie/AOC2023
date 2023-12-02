const INPUT1: &str = include_str!("../input");

const RED: u32 = 12;
const GREEN: u32 = 13;
const BLUE: u32 = 14;

fn main() {
    println!(
        "{}",
        INPUT1
            .lines()
            .enumerate()
            .filter_map(|(idx, line)| {
                let mut num: u32 = 0;
                let mut red = 0;
                let mut green = 0;
                let mut blue = 0;
                for ch in line.chars() {
                    match ch {
                        digit if ch.is_digit(10) => num = num * 10 + digit.to_digit(10).unwrap(),
                        'r' => {
                            if num != 0 {
                                red = num;
                                num = 0;
                            }
                        }
                        'b' => {
                            blue = num;
                            num = 0;
                        }
                        'g' => {
                            green = num;
                            num = 0
                        }
                        ':' => num = 0,
                        ';' => {
                            red = 0;
                            blue = 0;
                            green = 0;
                        }
                        _ => continue,
                    };
                    if red > RED || green > GREEN || blue > BLUE {
                        return None;
                    }
                }
                Some(idx + 1)
            })
            .sum::<usize>()
    );

    println!(
        "{}",
        INPUT1
            .lines()
            .filter_map(|line| {
                let mut num: u32 = 0;
                let mut set = vec![];
                let mut red = 0;
                let mut green = 0;
                let mut blue = 0;
                for ch in line.chars() {
                    match ch {
                        digit if ch.is_digit(10) => num = num * 10 + digit.to_digit(10).unwrap(),
                        'r' => {
                            // g[r]een
                            if num != 0 {
                                red = num;
                                num = 0;
                            }
                        }
                        'b' => {
                            blue = num;
                            num = 0;
                        }
                        'g' => {
                            green = num;
                            num = 0
                        }
                        ':' => num = 0,
                        ';' => {
                            set.push((red, green, blue));
                            red = 0;
                            blue = 0;
                            green = 0;
                            num = 0;
                        }
                        _ => continue,
                    };
                }
                set.push((red, green, blue));
                if set.is_empty() {
                    None
                } else {
                    let ret = set
                        .iter_mut()
                        .reduce(|acc, rgb| {
                            acc.0 = acc.0.max(rgb.0);
                            acc.1 = acc.1.max(rgb.1);
                            acc.2 = acc.2.max(rgb.2);
                            acc
                        })
                        .unwrap();
                    Some(ret.0 * ret.1 * ret.2)
                }
            })
            .sum::<u32>()
    )
}
