#include "gui.h"

std::unordered_map<std::string, ComplexVariableControl> variableControls;

const std::unordered_set<std::string> BUILT_IN_FUNCTIONS = {
    "abs", "exp", "log", "ln", "conj",
    "sqrt", "mod", "real", "imag",
    "sin", "asin", "asinh", "sinh",
    "cos", "acos", "acosh", "cosh",
    "tan", "atan", "atanh", "tanh",
    "z", "c"
};

static const NamedEquation equationTypes[] = {
    { "Mandelbrot Set - z^2 + c", "z^2 + c" },
    { "Julia Set - z^2 + juliaC", "z^2 + juliaC" },
    { "Newton's Fractal - z - (z^3 - 1) / (3*z^2)", "z - (z^3 - 1) / (3*z^2)" },
    { "Burning Ship Fractal - abs(z)^2 - c", " abs(z)^2 - c" },
    { "Tricorn Fractal - conj(z)^2 + c", "conj(z) ^ 2 + c" },
    { "Sine Fractal - sin(z) + c", "sin(z) + c" },
    { "Cosine Fractal - cos(z) + c", "cos(z) + c" },
    { "Exponential Fractal - exp(z) + c", "exp(z) + c" },
    { "Custom Equation", "z" }
};

const char* labels[] = {
    "Mandelbrot Set - z^2 + c",
    "Julia Set - z^2 + juliaC",
    "Newton's Fractal - z - (z^3 - 1) / (3*z^2)",
    "Burning Ship Fractal - abs(z)^2 - c",
    "Tricorn Fractal - conj(z)^2 + c",
    "Sine Fractal - sin(z) + c",
    "Cosine Fractal - cos(z) + c",
    "Exponential Fractal - exp(z) + c",
    "Custom Equation"
};

void setupGUI(GLFWwindow* window) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO();
	io.Fonts->AddFontFromFileTTF("resources/arialroundedmtbold.ttf", 16.0f);
	io.FontDefault = io.Fonts->Fonts.back();
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, false);
	ImGui_ImplOpenGL3_Init("#version 330");
}

void createFrame() {
	ImGui_ImplOpenGL3_NewFrame();
	ImGui_ImplGlfw_NewFrame();
	ImGui::NewFrame();

	ImGui::SetNextWindowPos(ImVec2(OPENGL_WIDTH, 0));
	ImGui::SetNextWindowSize(ImVec2(IMGUI_WIDTH, HEIGHT));
}

void renderFrame() {
	ImGui::Render();
	ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void removeFrame() {
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
}

std::string translateEquationToGLSL(const std::string& equation) {
    try {
        ComplexExpressionParser parser;
        return parser.translate(equation);
    }
    catch (const std::exception& e) {
        return "ERROR: " + std::string(e.what());
    }
}

std::vector<std::string> extractVariables(const std::string& equation) {
	std::vector<std::string> variables;
	std::regex variableRegex(R"(\b[a-zA-Z_][a-zA-Z0-9_]*\b)");
	auto wordsBegin = std::sregex_iterator(equation.begin(), equation.end(), variableRegex);
	auto wordsEnd = std::sregex_iterator();

	for (std::sregex_iterator it = wordsBegin; it != wordsEnd; ++it) {
		std::string match = it->str();

		if (BUILT_IN_FUNCTIONS.find(match) == BUILT_IN_FUNCTIONS.end()) {
			variables.push_back(match);
		}
	}

	return variables;
}

void updateVariableExistence(const std::vector<std::string>& variables) {
    std::unordered_set<std::string> current_vars(variables.begin(), variables.end());

    for (auto it = variableControls.begin(); it != variableControls.end(); ) {
        if (current_vars.count(it->first)) {
            ++it;
        }
        else {
            it = variableControls.erase(it);
        }
    }

    for (const auto& var : variables) {
        if (variableControls.find(var) == variableControls.end()) {
            variableControls[var] = { {0.0f, 0.0f} , false };
        }
    }
}

void createVariableSliders() {
    for (auto& [name, control] : variableControls) {
        ImGui::PushID(name.c_str());
        ImGui::Text("%s", name.c_str());
        ImGui::Checkbox("Constant", &control.isConstant);

        if (control.isConstant) ImGui::BeginDisabled();

        ImGui::Text("Real:");
        ImGui::SameLine();
        ImGui::DragFloat("##Real", &control.value.x, 0.005f, -10000.0f, 10000.0f, "%.4f");

        ImGui::Text("Imag:");
        ImGui::SameLine();
        ImGui::DragFloat("##Imag", &control.value.y, 0.005f, -10000.0f, 10000.0f, "%.4f");

        if (control.isConstant) ImGui::EndDisabled();

        ImGui::PopID();
        ImGui::Separator();
    }
}

void applyEquationTwice(Shader& fractalShader, const char* equation) {
    // For some reason, the functions needs to be applied twice for OpenGL and ImGUI to sync up.

    for (int i = 0; i < 2; ++i) {
        auto variables = extractVariables(equation);
        updateVariableExistence(variables);
        auto customEquation = translateEquationToGLSL(equation);
        fractalShader.reload(variables, customEquation);
    }
}

void componentsForGUI(Shader& fractalShader) {
    static int currentEquationIndex = 0;
    static std::unordered_map<std::string, float> variableValues;
    static char equation[256] = "z^2 + c";

    static bool initialized = false;

    if (!initialized) {
        applyEquationTwice(fractalShader, equation);
        initialized = true;
    }

    ImGui::Begin("Fractal Visualizer Controls", nullptr, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize);

    if (ImGui::CollapsingHeader("Color Stops", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent(20.0f);

        for (int i = 0; i < colorStops.size(); ++i) {
            ImGui::PushID(i);
            ImGui::ColorEdit3(("Color " + std::to_string(i + 1)).c_str(), &colorStops[i].x);
            ImGui::SliderFloat(("Stop " + std::to_string(i + 1)).c_str(), &stopPositions[i], 0.0f, 1.0f, "%.2f");
            ImGui::Spacing();
            ImGui::PopID();
        }
        ImGui::Spacing();
        ImGui::Separator();

        ImGui::Unindent();
    }

    if (ImGui::CollapsingHeader("Visual Settings", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent(20.0f);

        ImGui::SliderFloat("Contrast", &contrast, 0.1f, 5.0f, "%.2f");
        ImGui::SliderFloat("Zoom", &zoom, 0.000001f, 10.0f, "%.6f");
        ImGui::SliderInt("Iterations", &iterations, 10, 1000);
        ImGui::DragFloat("Escape Radius", &escapeRadius, 0.005f, 0.0, 10000.0f, "%.4f");
        
        if (ImGui::Button("Reset")) {
            contrast = 0.5f;
            zoom = 1.0f;
            iterations = 100;
            escapeRadius = 5.0f;
            centerX = 0.0;
            centerY = 0.0;
        }

        ImGui::Spacing();
        ImGui::Separator();

        ImGui::Unindent();
    }

    if (ImGui::CollapsingHeader("Equation Editor", ImGuiTreeNodeFlags_DefaultOpen)) {
        ImGui::Indent(20.0f);

        if (ImGui::CollapsingHeader("Operations and Functions")) {
            static std::vector<std::string> functions = {
                "+", "-", "*", "/",
                "^", "z", "c", "abs",
                "exp", "log", "ln", "conj",
                "sqrt", "mod", "real", "imag",
                "sin", "asin", "asinh", "sinh",
                "cos", "acos", "acosh", "cosh",
                "tan", "atan", "atanh", "tanh"
            };

            ImGui::Columns(4, nullptr, false);
            for (int i = 0; i < functions.size(); ++i) {
                ImGui::TextUnformatted(functions[i].c_str());
                ImGui::NextColumn();
            }
            ImGui::Columns(1);
        }
        ImGui::Spacing();
        ImGui::Separator();

        ImGui::Text("Equation Type: ");
        ImGui::SameLine();

        if (ImGui::Combo("##EquationType", &currentEquationIndex, labels, IM_ARRAYSIZE(labels))) {
            strncpy_s(equation, equationTypes[currentEquationIndex].expression, sizeof(equation));
            applyEquationTwice(fractalShader, equation);
        }

        ImGui::Spacing();
        ImGui::Text("Equation: z =");
        ImGui::SameLine();
        ImGui::InputText("##EquationInput", equation, IM_ARRAYSIZE(equation));

        ImGui::SameLine();
        if (ImGui::Button("Apply")) {
            if (strlen(equation) > 0) {
                applyEquationTwice(fractalShader, equation);
                currentEquationIndex = IM_ARRAYSIZE(labels) - 1;
            }
        }

        ImGui::Spacing();
        createVariableSliders();

        ImGui::Unindent();
    }

    ImGui::End();
}
