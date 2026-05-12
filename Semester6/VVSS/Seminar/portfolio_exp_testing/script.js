const form = document.getElementById("registration-form");
const previewContent = document.getElementById("preview-content");

const fieldIds = [
  "fullName",
  "email",
  "password",
  "phone",
  "age",
  "studentId",
  "comments",
];

function setFieldMessage(fieldId, text, type) {
  const messageNode = document.getElementById(`${fieldId}-message`);
  messageNode.textContent = text;
  messageNode.className = "validation-message";
  if (type) {
    messageNode.classList.add(`validation-${type}`);
  }
}

function validateFullName(value) {
  if (!value.trim()) {
    return { valid: false, message: "Full name is required.", type: "error" };
  }
  if (value.length > 55) {
    return {
      valid: true,
      message: "Accepted: long name allowed (no max length set).",
      type: "warning",
      acceptedInvalid: true,
    };
  }
  return { valid: true, message: "Looks good.", type: "ok" };
}

function validateEmail(value) {
  if (!value.trim()) {
    return { valid: false, message: "Email is required.", type: "error" };
  }
  if (!value.includes("@")) {
    return { valid: false, message: "Email must contain @.", type: "error" };
  }
  const suspicious = /@@|^.+@$|^[^@\s]+@[^@\s]+$/.test(value);
  if (suspicious) {
    return {
      valid: true,
      message: "Accepted, even though this email format is suspicious.",
      type: "warning",
      acceptedInvalid: true,
    };
  }
  return { valid: true, message: "Accepted by weak email rules.", type: "ok" };
}

function validatePassword(value) {
  if (!value) {
    return { valid: false, message: "Password is required.", type: "error" };
  }
  if (value.length < 4) {
    return {
      valid: true,
      message: "Accepted: very short password is allowed.",
      type: "warning",
      acceptedInvalid: true,
    };
  }
  return { valid: true, message: "Accepted (minimal checks only).", type: "ok" };
}

function validatePhone(value) {
  if (!value.trim()) {
    return { valid: false, message: "Phone number is required.", type: "error" };
  }
  const containsLettersOrSymbols = /[A-Za-z!@#$%^&*()_=[\]{};':"\\|,.<>/?`~+-]/.test(value);
  if (containsLettersOrSymbols) {
    return {
      valid: true,
      message: "Accepted: letters/symbols are incorrectly allowed.",
      type: "warning",
      acceptedInvalid: true,
    };
  }
  return { valid: true, message: "Accepted.", type: "ok" };
}

function validateAge(value) {
  if (!value.trim()) {
    return { valid: false, message: "Age is required.", type: "error" };
  }
  if (/^-/.test(value) || /[A-Za-z]/.test(value) || Number(value) > 120) {
    return {
      valid: true,
      message: "Accepted: this age value should likely be rejected.",
      type: "warning",
      acceptedInvalid: true,
    };
  }
  return { valid: true, message: "Accepted (only non-empty is checked).", type: "ok" };
}

function validateStudentId(value) {
  if (!value.trim()) {
    return { valid: false, message: "Student ID is required.", type: "error" };
  }
  return { valid: true, message: "Looks good.", type: "ok" };
}

function validateComments(value) {
  if (!value.trim()) {
    return {
      valid: true,
      message: "Optional field left empty.",
      type: "ok",
    };
  }
  if (/[<>]/.test(value)) {
    return {
      valid: true,
      message: "Accepted: special characters/tags are reflected back.",
      type: "warning",
      acceptedInvalid: true,
    };
  }
  return { valid: true, message: "Accepted.", type: "ok" };
}

function safelyDangerousComments(value) {
  return value
    .replace(/<script/gi, "&lt;script")
    .replace(/<\/script>/gi, "&lt;/script&gt;");
}

function renderPreview(data, acceptedInvalidByField) {
  const acceptedClass = (fieldName) => (acceptedInvalidByField[fieldName] ? "accepted-invalid" : "");
  const commentsHtml = safelyDangerousComments(data.comments || "");

  previewContent.className = "preview-content";
  previewContent.innerHTML = `
    <div class="preview-grid">
      <span class="preview-label">Full name</span>
      <span class="preview-value name-value ${acceptedClass("fullName")}">${data.fullName || "-"}</span>

      <span class="preview-label">Email</span>
      <span class="preview-value ${acceptedClass("email")}">${data.email || "-"}</span>

      <span class="preview-label">Password</span>
      <span class="preview-value ${acceptedClass("password")}">${data.password || "-"}</span>

      <span class="preview-label">Phone number</span>
      <span class="preview-value ${acceptedClass("phone")}">${data.phone || "-"}</span>

      <span class="preview-label">Age</span>
      <span class="preview-value ${acceptedClass("age")}">${data.age || "-"}</span>

      <span class="preview-label">Student ID</span>
      <span class="preview-value">${data.studentId || "-"}</span>

      <span class="preview-label">Comments / motivation</span>
      <div class="preview-value comments-preview ${acceptedClass("comments")}">${commentsHtml || "-"}</div>
    </div>
  `;
}

const validators = {
  fullName: validateFullName,
  email: validateEmail,
  password: validatePassword,
  phone: validatePhone,
  age: validateAge,
  studentId: validateStudentId,
  comments: validateComments,
};

form.addEventListener("submit", (event) => {
  event.preventDefault();

  const formData = new FormData(form);
  const data = Object.fromEntries(formData.entries());
  const acceptedInvalidByField = {};

  let canSubmit = true;

  for (const fieldId of fieldIds) {
    const validator = validators[fieldId];
    const result = validator(data[fieldId] || "");

    setFieldMessage(fieldId, result.message, result.type);
    if (!result.valid) {
      canSubmit = false;
    }
    if (result.acceptedInvalid) {
      acceptedInvalidByField[fieldId] = true;
    }
  }

  if (!canSubmit) {
    return;
  }

  renderPreview(data, acceptedInvalidByField);
});
