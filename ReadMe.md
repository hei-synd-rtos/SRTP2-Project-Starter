# SRTP2-Project (RTOS)

Clone the repository from Github and then run the commands to build the project:

```bash 
cd SRTP2-Project-xxx/srtp2-project/app
west update
cd ../..
python3 -m venv .venv
source .venv/bin/activate
pip install -r zephyr/scripts/requirements.txt
cd srtp2-project
west build -b stm32f746g-disco app
west flash
```

You should now see LD0 blinking with a frequency of about 2Hz.
