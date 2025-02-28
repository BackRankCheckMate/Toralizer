# Toralizer
This tool transforms Linux Command (such as curl) into a TOR network client and ensures that your activities are anonymous and secure.

However, be careful while using this. Only the IP connection will be private, not the DNS request.

## Usage
```bash
bash_script_name curl http://IP_ADDRESS_OF_A_WEBSITE_OR_SERVER
Example:
toralize curl http://142.250.207.206
# The above IP address belongs to Google
```

## Final Notes
- This technique is useful for anonymous browsing and bypassing restrictions
- Limitations:
  - Does not Support SOCKS5
  - May fail on IPV6 as SOCKS4 does not support it
