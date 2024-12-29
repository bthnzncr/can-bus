// chart.js

import Chart from 'https://cdn.jsdelivr.net/npm/chart.js';

// Grafik nesneleri için bir global yapı
const charts = {};
const chartData = {};

// Grafik oluşturma fonksiyonu
export function createChart(chartId, label, labels = [], values = []) {
    const ctx = document.getElementById(chartId).getContext('2d');

    // Başlangıçta etiket ve veri dizisi boş ise, varsayılan değerler ekleyin
    const initialLabels = labels.length > 0 ? labels : ['00:00:00'];
    const initialValues = values.length > 0 ? values : [0];

    chartData[chartId] = {
        labels: initialLabels,
        values: initialValues,
    };

    charts[chartId] = new Chart(ctx, {
        type: 'line',
        data: {
            labels: chartData[chartId].labels,
            datasets: [
                {
                    label: label,
                    data: chartData[chartId].values,
                    borderColor: '#007BFF',
                    backgroundColor: 'rgba(0, 123, 255, 0.1)',
                    borderWidth: 2,
                    tension: 0.2,
                    fill: true,
                },
            ],
        },
        options: {
            responsive: true,
            plugins: {
                legend: {
                    display: true,
                },
            },
            scales: {
                x: {
                    type: 'time',
                    time: {
                        unit: 'second',
                        displayFormats: {
                            second: 'HH:mm:ss',
                        },
                    },
                    title: {
                        display: true,
                        text: 'Time',
                    },
                },
                y: {
                    title: {
                        display: true,
                        text: 'Value',
                    },
                },
            },
        },
    });
}

// Grafik verisini güncelleme fonksiyonu
export function updateChart(chartId, newLabel, newValue) {
    if (!charts[chartId]) return;

    const maxDataPoints = 50; // En fazla tutulacak veri sayısı

    // Zaman etiketini ve yeni değeri ekle
    chartData[chartId].labels.push(newLabel);
    chartData[chartId].values.push(newValue);

    // Fazla veriyi çıkar
    if (chartData[chartId].labels.length > maxDataPoints) {
        chartData[chartId].labels.shift();  // İlk etiketi sil
        chartData[chartId].values.shift();  // İlk veriyi sil
    }

    charts[chartId].update();
}
