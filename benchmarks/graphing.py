import pandas as pd
import matplotlib.pyplot as plt
import seaborn as sns

data = pd.read_csv('./benchmarks/benchmark_results.csv')

mean_times = data.groupby(['num_processors', 'topology', 'load_balanced'])['time'].mean().reset_index()

sns.set_theme(style="whitegrid")

fig, ax = plt.subplots(figsize=(14, 8))

sns.lineplot(x='num_processors', y='time', hue='topology', style='load_balanced', markers=True, dashes=False, 
             data=data, legend=False, alpha=0.5, linewidth=0.75, ax=ax)

sns.lineplot(x='num_processors', y='time', hue='topology', style='load_balanced', markers=True, dashes=True, 
             data=mean_times, ax=ax)

plt.title('Benchmark Results: Time vs Number of Processors')
plt.xlabel('Number of Processors')
plt.ylabel('Time (seconds)')
plt.legend(title='Configuration')
plt.grid(True)

plt.savefig('./benchmarks/result.png')

plt.show()

