# Student Registration Form - Exploratory Testing Demo

This is a small intentionally buggy demo app for recording a **3 to 5 minute exploratory testing video** with the **Bug Magnet** browser extension.

## Setup

1. Open a terminal in this project folder.
2. Run:

```bash
python3 -m http.server 5500
```

3. Open [http://localhost:5500](http://localhost:5500) in your browser.

## Recommended Browser

- Chrome or Firefox (for Bug Magnet support)

## Purpose

The app contains intentional validation defects so Bug Magnet can quickly surface findings:

- Weak email validation
- Missing max length for full name
- Unsafe reflection of comments
- Permissive phone validation
- Weak password rules
- Incorrect age validation

## Recording Guide (3 to 5 minutes)

1. Install Bug Magnet in Chrome or Firefox.
2. Open the local demo app.
3. Right-click inside form fields.
4. Use Bug Magnet values for emails, names, special characters, numbers, and security strings.
5. Submit the form after each test.
6. Explain what happened and why it is a finding.

## Video Script with Timestamps

### 0:00 to 0:30 - Introduction

- Show the page title: **Student Registration Form**
- Explain this is a controlled demo app for exploratory testing with intentional validation defects.

### 0:30 to 1:00 - Show Bug Magnet Setup

- Show Bug Magnet installed in browser.
- Right-click a form field and open the Bug Magnet menu.
- Mention this accelerates edge-case input generation.

### 1:00 to 3:30 - Test Fields with Bug Magnet Values

- **Email**: insert values like `a@`, `test@@mail`, `abc@abc`; show they are accepted.
- **Full name**: insert very long string; show layout stretches and no max length enforcement.
- **Password**: try very short value like `1`; show it gets accepted.
- **Phone number**: use alphabetic/symbol-heavy input; show acceptance.
- **Age**: try `-5`, `999`, `abc`; show accepted as long as not empty.
- **Comments**: insert HTML/security string like `<script>alert(1)</script>` or tags; show unsafe reflection in preview panel.

### 3:30 to 4:30 - Summarize Findings

- Recap each discovered validation defect.
- Point to field messages and the submitted data preview as evidence.

### 4:30 to 5:00 - Explain Benefits

- Bug Magnet speeds up exploratory testing by providing quick edge-case inputs.
- It helps uncover validation gaps, unsafe reflection behavior, and weak business rules faster.
- It improves tester efficiency and coverage during short test sessions.

## Notes

- This app is intentionally insecure and should **not** be used as a production template.
