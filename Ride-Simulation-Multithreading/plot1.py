import matplotlib.pyplot as plt
import pandas as pd
df1 = pd.read_csv("./passenger_avg_tour_time.csv")
print(df1.info())

# Plot-1
plt.plot(df1["no_of_passengers"],df1["avg_tour_time"],marker="^",label='Avg tour time for passengers')
plt.xlabel("No. of passengers")
plt.ylabel("Avg time for tour for passengers(milliseconds)")
plt.title("Avg time for tour for passengers vs No. of passengers")
plt.tight_layout()
plt.grid(True)
plt.legend()
plt.savefig("./avg_passenger.png")
plt.show()