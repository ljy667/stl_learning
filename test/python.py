# -*- coding: UTF-8 -*-
'''
@Project ：视频转音频 
@File    ：格式转换.py
@IDE     ：PyCharm 
@Author  ：一晌小贪欢 (278865463@qq.com)
@Date    ：2024/9/1 0:06
'''

import tkinter as tk
from tkinter import filedialog
import os
import ffmpy

def convert_video():
    source_file = entry_source.get()
    output_dir = entry_output_dir.get()
    base_name = os.path.splitext(os.path.basename(source_file))[0]
    sink_file = os.path.join(output_dir, f"{base_name}.mp4")

    ff = ffmpy.FFmpeg(
        executable=entry_ffmpeg.get(),
        inputs={source_file: None},
        outputs={sink_file: '-y'})  # 使用-y参数覆盖输出文件，如果存在的话
    ff.run()
    status_label.config(text="转换完成")

def select_file(entry):
    file_path = filedialog.askopenfilename()
    entry.delete(0, tk.END)
    entry.insert(0, file_path)

def select_output_directory(entry):
    dir_path = filedialog.askdirectory()
    entry.delete(0, tk.END)
    entry.insert(0, dir_path)

root = tk.Tk()
root.title("视频转换器")

frame = tk.Frame(root)
frame.pack(pady=20)

label_source = tk.Label(frame, text="源文件:")
label_source.grid(row=0, column=0)
entry_source = tk.Entry(frame, width=50)
entry_source.grid(row=0, column=1)
button_source = tk.Button(frame, text="浏览", command=lambda: select_file(entry_source))
button_source.grid(row=0, column=2)

label_output_dir = tk.Label(frame, text="输出目录:")
label_output_dir.grid(row=1, column=0)
entry_output_dir = tk.Entry(frame, width=50)
entry_output_dir.grid(row=1, column=1)
button_output_dir = tk.Button(frame, text="选择目录", command=lambda: select_output_directory(entry_output_dir))
button_output_dir.grid(row=1, column=2)

label_ffmpeg = tk.Label(frame, text="FFmpeg路径:")
label_ffmpeg.grid(row=2, column=0)
entry_ffmpeg = tk.Entry(frame, width=50)
entry_ffmpeg.insert(0, r"D:\ffmpeg\ffmpeg.exe")
entry_ffmpeg.grid(row=2, column=1)

convert_button = tk.Button(frame, text="开始转换", command=convert_video)
convert_button.grid(row=3, column=1)

status_label = tk.Label(frame, text="", fg="green")
status_label.grid(row=4, column=1)

root.mainloop()
