pragma Singleton
import QtQuick 2.15

QtObject {
    readonly property string iconFile: "📄"
    readonly property string iconDrop: "📥"
    readonly property string iconFolder: "📁"
    readonly property string iconAdd: "➕"
    readonly property string iconDelete: "✕"
    readonly property string iconWarning: "⚠️"
    readonly property string iconSuccess: "✅"
    readonly property string iconError: "❌"
    readonly property string iconImagesToPdf: "🖼️"
    readonly property string iconMergePdf: "📚"
    readonly property string iconEdit: "📝"

    readonly property color primary: "#3498db"       // Основной синий
    readonly property color primaryDark: "#2980b9"   // Синий при нажатии
    readonly property color danger: "#e74c3c"        // Красный (удаление/ошибки)

    readonly property color bgMain: "#ffffff"        // Фон окна
    readonly property color bgLight: "#f8f9fa"       // Фон панелей

    readonly property color borderDefault: "#dee2e6" // Светлая
    readonly property color borderHover: "#adb5bd"   // Темно-серая

    readonly property color textMain: "#2c3e50"      // Основной текст
    readonly property color textSecondary: "#7f8c8d" // Серый текст (размеры, логи)

    readonly property int radius: 6                  // Единый радиус для всего
    readonly property int spacing: 10


    readonly property double ratioA4: 1.4142
    readonly property int cardWidth: 180
}
