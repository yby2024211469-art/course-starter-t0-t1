# T0 证据

记录人：殷白玉，2024211469。五人团队成员，组长为蔡融汉。

- build_log_2026-10-10.txt、environment_2026-10-10.txt：编译及环境检查。
- hook_reject.txt、hook_reject_missing_validation.txt：两次不合规提交被拒绝的输出。
- flash_log_20261010_133701.txt、flash_screenshot_20261010.png：殷白玉执行烧录并提供截图。
- serial_running_20261010.txt：Codex 在 COM9 上只读采集的运行日志。
- rgb_video_20261010.mp4：殷白玉提供的视频；本人确认红绿蓝循环正常。视频不由 Git 跟踪，交付时需附上此文件。

视频大小：910986 字节。SHA256：11BA41B66E436F2483152B5BBE0E4E374DF0183B49C226B1139B9235A8EEB472。

今天使用的是同型号的另一块板。烧录正常结束，RGB 三色循环由殷白玉确认，串口记录也有对应输出。

之后殷白玉运行启动采集脚本，保存 boot_log_20261010_140620.txt 并回传 boot_screenshot_20261010.png。原始日志包含 `[course_starter] v0.1.0 boot ok`、`Hello world!` 和红绿蓝循环，启动证据已补齐。
