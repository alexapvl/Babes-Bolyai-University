from selenium import webdriver
from selenium.webdriver.common.by import By
import time

# Launch Chrome with password leak detection disabled
options = webdriver.ChromeOptions()
options.add_argument("--password-store=basic")
options.add_experimental_option("prefs", {
    "credentials_enable_service": False,
    "profile.password_manager_enabled": False,
    "profile.password_manager_leak_detection": False
})
driver = webdriver.Chrome(options=options)
driver.maximize_window()
# driver.set_window_size(1024, 768)

# Step 1: Open the login page
driver.get("https://the-internet.herokuapp.com/login")
time.sleep(9)
driver.save_screenshot("01_login_page.png")
print("Screenshot 1: Login page")

# Step 2: Fill in the username
username_field = driver.find_element(By.ID, "username")
username_field.send_keys("tomsmith")
time.sleep(10)
driver.save_screenshot("02_username_filled.png")
print("Screenshot 2: Username entered")

# Step 3: Fill in the password
password_field = driver.find_element(By.ID, "password")
password_field.send_keys("SuperSecretPassword!")
time.sleep(5)
driver.save_screenshot("03_form_filled.png")
print("Screenshot 3: Full form filled")

# Step 4: Click Login
login_button = driver.find_element(By.CSS_SELECTOR, "button[type='submit']")
login_button.click()
time.sleep(10)
driver.save_screenshot("04_login_success.png")
print("Screenshot 4: Login success page")

# Step 5: Logout
logout_button = driver.find_element(By.CSS_SELECTOR, "a[href='/logout']")
logout_button.click()
time.sleep(7)
driver.save_screenshot("05_logged_out.png")
print("Screenshot 5: Logged out")

print("Demo complete! All screenshots saved.")
driver.quit()
