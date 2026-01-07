import csv
import matplotlib.pyplot as plt
import sys

def plot_prices(infile, outfile):
    prices = []

    with open(infile, newline="") as f:
        reader = csv.DictReader(f)
        for row in reader:
            if int(row["price"]) != 0:
                prices.append(int(row["price"]) / 10_000)

    plt.figure()
    plt.plot(prices)
    plt.xlabel("Index")
    plt.ylabel("Price")
    plt.title("Price Distribution")
    plt.grid(True)

    plt.tight_layout()
    plt.savefig(outfile, dpi=300)
    plt.show()

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("python plot_prices.py [prices csv] [path to output file]")
        sys.exit(1)

    infile = sys.argv[1];
    outfile = sys.argv[2];

    plot_prices(infile, outfile)
