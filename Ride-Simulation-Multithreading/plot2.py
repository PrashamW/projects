import matplotlib.pyplot as plt
import pandas as pd
df1 = pd.read_csv("./car_avg_tour_time.csv")
print(df1.info())

# Plot-1
plt.plot(df1["no_of_cars"],df1["avg_tour_time"],marker="^",label='Avg tour time for cars')
plt.xlabel("No. of cars")
plt.ylabel("Avg time for tour for cars(milliseconds)")
plt.title("Avg time for tour for cars vs No. of cars")
plt.tight_layout()
plt.grid(True)
plt.legend()
plt.savefig("./avg_car.png")
plt.show()