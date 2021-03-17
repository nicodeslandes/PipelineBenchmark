use rand::prelude::*;
use std::time::Instant;

fn main() {
    println!("Allocating 10GB array");
    let mut arr: Vec<i64> = vec![0; 10_000_000_000 / 8];
    println!("Populating array with random data");
    let mut rng = rand::thread_rng();
    for iter in arr.iter_mut() {
        *iter = rng.next_u64() as i64;
    }
    let arr = arr;

    println!("Calculating sum of positive elements");

    for _ in 0..10 {
        let start = Instant::now();
        let sum: i64 = arr.iter().filter(|&x| *x > 0).sum();
        println!("Result: {}, in {} ms", sum, start.elapsed().as_millis());
    }
}
