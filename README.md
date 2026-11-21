<div align="center">

# 🌉 Pol Cloudflare Bridge
### Android client and Cloudflare bridge for AI services
**In-app browser · Offline text models · 3D interface**

<br/>
<a href="https://github.com/imgutss/pol-cloudflare-bridge/releases/download/v1.0.1/Pol-Cloudflare-Bridge.apk"><img src="https://img.shields.io/badge/Download%20APK-v1.0.1-FF8A2A?style=for-the-badge&logo=android&logoColor=white" alt="Download APK" /></a>
<a href="./README_FA.md"><img src="https://img.shields.io/badge/راهنمای%20فارسی-Persian%20Docs-2563EB?style=for-the-badge" alt="Persian Documentation" /></a>
<a href="https://github.com/imgutss/pol-cloudflare-bridge/stargazers"><img src="https://img.shields.io/badge/⭐%20Star-Support%20Project-F59E0B?style=for-the-badge" alt="Stars" /></a>

<br/><br/>
<img src="https://img.shields.io/badge/Android-8%2B-3DDC84?style=flat-square&logo=android&logoColor=white" />
<img src="https://img.shields.io/badge/Cloudflare-Workers-F38020?style=flat-square&logo=cloudflare&logoColor=white" />
<img src="https://img.shields.io/badge/Offline-ARM64%20CPU-8B5CF6?style=flat-square" />

</div>

---

## 🌟 Overview

Pol Cloudflare Bridge is an Android app with an in-app browser, AI service connections and offline text chat. The browser path uses a local proxy and a configurable Cloudflare Worker bridge.

| Feature | Description |
| :--- | :--- |
| 🌐 **In-app browser** | Open services such as Gemini and Muse within the app. |
| 🔧 **Manual API configuration** | Enter the API URL, model name and API key manually; automatic model selection is disabled. |
| 🤖 **Offline text models** | Download compatible models separately and run them on the device's CPU. |
| 💬 **Progressive replies** | Incremental offline responses, retaining a loaded model between messages, and Markdown rendering. |
| 🗂️ **Model management** | Horizontal model cards with an option to remove a local model. |
| ⌨️ **Gemini keyboard** | Adjusted keyboard inset handling in the in-app browser. |

## 🚀 Download & installation

| Release | APK size | Requirements | Download |
| :---: | :---: | :---: | :---: |
| **v1.0.1** | ~168 MB | Android 8+; ARM64 required for offline inference | [📥 Download APK](https://github.com/imgutss/pol-cloudflare-bridge/releases/download/v1.0.1/Pol-Cloudflare-Bridge.apk) |

Offline models are not bundled; they require separate downloads, storage and sufficient memory. Back up important data before installing. Android may refuse an in-place update if the installed APK has a different signing key; uninstalling can erase app data.

## ☁️ Configure the bridge

Deploy your own Worker using [`worker/`](./worker/) and enter its URL and bridge key in the app. For a custom service, enter its API URL, model name and API key manually.

> Cloudflare's outbound country and IP are not guaranteed. Availability depends on the service and network. Not every provider or app path has been verified to use the tunnel. The custom adapter does not fall back to a direct connection when its bridge is unavailable.

## ℹ️ Release limitations

- Offline image/camera input and GPU acceleration are not included; offline text inference uses CPU.
- Compatibility, speed and memory requirements depend on the model and device. The Gemini keyboard was tested by the user on a device, not on all devices and WebView versions.
- This repository contains earlier sources/resources and selected patches for this release; it **does not currently contain the complete source and independent build process** for the published APK.

📋 [Changelog](./CHANGELOG.md) · 📦 [All releases](https://github.com/imgutss/pol-cloudflare-bridge/releases) · 🔒 [Security](./SECURITY.md)

---
<div align="center"><b>Developed by <a href="https://github.com/imgutss">Guts (imgutss)</a></b></div>
