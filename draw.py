import matplotlib.pyplot as plt

sizes = list(range(1, 101))

with open('insertion.txt', 'r', encoding='utf-8') as f:
    insertion_times = f.readlines()
with open('quicksort.txt', 'r', encoding='utf-8') as f:
    quick_times = f.readlines()

sorted(insertion_times)
sorted(quick_times)

sizes_step2 = sizes[::2]
insertion_times_step2 = insertion_times[::2]
quick_times_step2 = quick_times[::2]

plt.figure(figsize=(10, 6))

plt.plot(sizes_step2, quick_times_step2, 'r-', linewidth=2, label='quick sort')

plt.plot(sizes_step2, insertion_times_step2, 'b-', linewidth=2, label='insertion sort')

plt.xlabel('Array Size')
plt.ylabel('Execution Time (ms)')
plt.title('Sorting Algorithms Comparison')
plt.legend()
plt.grid(True)

plt.savefig('sorting_comparison.png', dpi=300)
plt.show()
