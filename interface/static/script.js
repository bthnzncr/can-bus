
        // DOM Elements
        const chartsContainer = document.getElementById('charts-container');
        const dataBody = document.getElementById('data-body');
        const statusElement = document.getElementById('status');
        const mainContent = document.querySelector('.main-content');
        const leftSidebar = document.querySelector('.left-sidebar');

        // Configuration
        const config = {
            updateInterval: 2000,
            maxDataPoints: 50,
            apiBaseUrl: 'http://35.246.196.214:8080',
            connectionTimeout: 10000 // 10 seconds
        };

        // State
        let charts = {};
        let lastUpdateTime = 0;
        let connectionCheckInterval = null;

        // Chart Configuration
        const chartColors = {
            temperature: 'rgb(255, 99, 132)',
            pressure: 'rgb(54, 162, 235)',
            speed: 'rgb(255, 206, 86)',
            percentage: 'rgb(75, 192, 192)',
            voltage: 'rgb(153, 102, 255)',
            default: 'rgb(255, 159, 64)'
        };

        // Define which PIDs should have charts and their configurations
        const chartConfigs = {
            'ENGINE_SPEED': { 
                unit: 'RPM',
                label: 'Engine Speed',
                color: chartColors.speed,
                min: 0,
                max: 8000
            },
            'VEHICLE_SPEED': {
                unit: 'km/h',
                label: 'Vehicle Speed',
                color: chartColors.speed,
                min: 0,
                max: 220
            },
            'ENGINE_COOLANT_TEMP': {
                unit: '°C',
                label: 'Coolant Temperature',
                color: chartColors.temperature,
                min: -20,
                max: 150
            },
            'ENGINE_OIL_TEMP': {
                unit: '°C',
                label: 'Oil Temperature',
                color: chartColors.temperature,
                min: -20,
                max: 150
            },
            'THROTTLE_POSITION': {
                unit: '%',
                label: 'Throttle Position',
                color: chartColors.percentage,
                min: 0,
                max: 100
            },
            'RELATIVE_THROTTLE_POSITION': {
                unit: '%',
                label: 'Relative Throttle',
                color: chartColors.percentage,
                min: 0,
                max: 100
            },
            'INTAKE_MANIFOLD_PRESSURE': {
                unit: 'kPa',
                label: 'Intake Pressure',
                color: chartColors.pressure,
                min: 0,
                max: 255
            },
            'INTAKE_AIR_TEMP': {
                unit: '°C',
                label: 'Intake Temperature',
                color: chartColors.temperature,
                min: -20,
                max: 150
            },
            'O2_VOLTAGE': {
                unit: 'V',
                label: 'O2 Sensor Voltage',
                color: chartColors.voltage,
                min: 0,
                max: 5
            }
        };

        // Skip these PIDs for charts
        const skipChartPIDs = ['FUEL_SYSTEM_STATUS', 'MAF_SENSOR'];

        function shouldCreateChart(name, value) {
            // Skip charts for non-numeric values and specific PIDs
            if (skipChartPIDs.includes(name)) return false;
            if (value.startsWith('0x')) return false;  // Skip hex values
            if (name === 'VIN') return false;
            if (name === 'FUEL_TYPE' || 
                name === 'OBD_STANDARDS' || 
                name === 'EMISSION_REQUIREMENTS') return false;
            
            // Only create charts for values that start with numbers and have a config
            const numericMatch = value.match(/^[\d.-]+/);
            return numericMatch !== null && chartConfigs.hasOwnProperty(name);
        }

        function createChart(name, value) {
            if (!shouldCreateChart(name, value)) return null;

            const config = chartConfigs[name] || {
                unit: extractUnit(value),
                label: name.replace(/_/g, ' '),
                color: chartColors.default,
                min: 0,
                max: 100
            };

            const chartDiv = document.createElement('div');
            chartDiv.className = 'chart-container';
            const canvas = document.createElement('canvas');
            canvas.id = `chart-${name}`;
            chartDiv.appendChild(canvas);
            chartsContainer.appendChild(chartDiv);

            // Add minimize button
            const minimizeBtn = document.createElement('button');
            minimizeBtn.className = 'minimize-btn';
            minimizeBtn.innerHTML = '-';
            minimizeBtn.onclick = () => toggleChartSize(chartDiv);
            chartDiv.appendChild(minimizeBtn);

            const ctx = canvas.getContext('2d');

            charts[name] = new Chart(ctx, {
                type: 'line',
                data: {
                    labels: [],
                    datasets: [{
                        label: config.label,
                        data: [],
                        borderColor: config.color,
                        backgroundColor: config.color.replace('rgb', 'rgba').replace(')', ', 0.1)'),
                        borderWidth: 2,
                        tension: 0.4,
                        fill: true,
                        pointRadius: 0
                    }]
                },
                options: {
                    responsive: true,
                    maintainAspectRatio: false,
                    animation: {
                        duration: 0
                    },
                    scales: {
                        x: {
                            type: 'category',
                            display: true,
                            grid: {
                                color: 'rgba(255, 255, 255, 0.1)'
                            },
                            ticks: {
                                color: '#cccccc',
                                maxTicksLimit: 5
                            }
                        },
                        y: {
                            display: true,
                            min: config.min,
                            max: config.max,
                            grid: {
                                color: 'rgba(255, 255, 255, 0.1)'
                            },
                            ticks: {
                                color: '#cccccc',
                                callback: function(value) {
                                    return value + ' ' + config.unit;
                                }
                            }
                        }
                    },
                    plugins: {
                        legend: {
                            display: true,
                            labels: {
                                color: '#cccccc'
                            }
                        },
                        title: {
                            display: true,
                            text: config.label,
                            color: '#cccccc',
                            font: {
                                size: 14
                            }
                        }
                    }
                }
            });

            return charts[name];
        }

        function toggleChartSize(chartDiv) {
            if (chartDiv.classList.contains('minimized')) {
                chartDiv.classList.remove('minimized');
                chartDiv.style.height = '300px';
            } else {
                chartDiv.classList.add('minimized');
                chartDiv.style.height = '50px';
            }
            // Trigger resize to update chart display
            window.dispatchEvent(new Event('resize'));
        }

        // Connection State Management
        function showNoConnectionState() {
            // Clear all data
            chartsContainer.innerHTML = '';
            dataBody.innerHTML = '';
            document.querySelector('.info-grid').innerHTML = '';
            document.querySelector('#vin-details').innerHTML = '';
            charts = {};

            // Show no connection message
            const noConnectionMsg = document.createElement('div');
            noConnectionMsg.className = 'no-connection-message';
            noConnectionMsg.innerHTML = `
                <div class="message-content">
                    <i class="fas fa-car"></i>
                    <h2>No Car Connected</h2>
                    <p>Waiting for vehicle connection...</p>
                </div>
            `;
            mainContent.insertBefore(noConnectionMsg, mainContent.firstChild);
            
            updateStatus('disconnected');
        }

        function hideNoConnectionState() {
            const noConnectionMsg = document.querySelector('.no-connection-message');
            if (noConnectionMsg) {
                noConnectionMsg.remove();
            }
        }

        function checkConnection() {
            const currentTime = Date.now();
            if (currentTime - lastUpdateTime > config.connectionTimeout) {
                showNoConnectionState();
            }
        }

        // Data fetching functions
        async function fetchVehicleInfo() {
            try {
                const response = await fetch(`${config.apiBaseUrl}/api/vehicle-info`);
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
                const data = await response.json();
                console.log('Vehicle Info Data:', data);
                if (data.pids) {
                    hideNoConnectionState();
                    data.pids.forEach(pid => updateVehicleInfo(pid.name, pid.value));
                    lastUpdateTime = Date.now();
                    updateStatus('connected');
                }
            } catch (error) {
                console.error('Error fetching vehicle info:', error);
                checkConnection();
            }
        }

        async function fetchRealtimeData() {
            try {
                const response = await fetch(`${config.apiBaseUrl}/api/realtime-data`);
                if (!response.ok) {
                    throw new Error(`HTTP error! status: ${response.status}`);
                }
                const data = await response.json();
                console.log('Realtime Data:', data);
                if (data.pids) {
                    hideNoConnectionState();
                    data.pids.forEach(pid => {
                        updateTableRow(pid.name, pid.value);
                        if (shouldCreateChart(pid.name, pid.value)) {
                            if (!charts[pid.name]) {
                                createChart(pid.name, pid.value);
                            }
                            updateChart(pid.name, pid.value);
                        }
                    });
                    lastUpdateTime = Date.now();
                    updateStatus('connected');
                }
            } catch (error) {
                console.error('Error fetching realtime data:', error);
                checkConnection();
            }
        }

        function updateStatus(state, message) {
            switch (state) {
                case 'connected':
                    statusElement.textContent = 'Connected';
                    statusElement.style.color = 'green';
                    break;
                case 'disconnected':
                    statusElement.textContent = 'No Car Connected';
                    statusElement.style.color = 'red';
                    break;
                case 'error':
                    statusElement.textContent = message || 'Connection error';
                    statusElement.style.color = 'red';
                    break;
            }
        }

        // Initialize the page
        document.addEventListener('DOMContentLoaded', () => {
            showNoConnectionState();
            
            // Start periodic data fetching
            setInterval(fetchVehicleInfo, 1000);
            setInterval(fetchRealtimeData, 1000);
            
            // Start connection checking
            setInterval(checkConnection, 1000);
        });

        function getChartColor(name, value) {
            if (value.includes('°C')) return chartColors.temperature;
            if (value.includes('kPa')) return chartColors.pressure;
            if (value.includes('km/h') || value.includes('rpm')) return chartColors.speed;
            if (value.includes('%')) return chartColors.percentage;
            if (value.includes('V')) return chartColors.voltage;
            return chartColors.default;
        }

        function extractNumericValue(value) {
            const numericMatch = value.match(/^[\d.-]+/);
            return numericMatch ? parseFloat(numericMatch[0]) : null;
        }

        function formatTableValue(name, value) {
            // For hex values, format them nicely
            if (value.startsWith('0x')) {
                return `<span class="hex-value">${value}</span>`;
            }
            
            // For string-only values, just return as is
            if (name === 'FUEL_TYPE' || 
                name === 'OBD_STANDARDS' || 
                name === 'EMISSION_REQUIREMENTS' ||
                name === 'VIN') {
                return value;
            }
            
            // For numeric values with units
            const numericValue = extractNumericValue(value);
            if (numericValue !== null) {
                const unit = value.replace(numericValue, '').trim();
                return `<span class="numeric-value">${numericValue}</span> <span class="unit">${unit}</span>`;
            }
            
            return value;
        }

        function updateTableRow(name, value) {
            let row = document.getElementById(`row-${name}`);
            if (!row) {
                row = document.createElement('tr');
                row.id = `row-${name}`;
                row.innerHTML = `
                    <td>${name.replace(/_/g, ' ')}</td>
                    <td class="value-cell"></td>
                `;
                requestAnimationFrame(() => {
                    dataBody.appendChild(row);
                });
            }
            
            const valueCell = row.querySelector('.value-cell');
            const newValue = formatTableValue(name, value);
            
            if (valueCell.innerHTML !== newValue) {
                requestAnimationFrame(() => {
                    valueCell.innerHTML = newValue;
                    valueCell.classList.remove('value-update');
                    void valueCell.offsetWidth;
                    valueCell.classList.add('value-update');
                });

                // Update chart if exists
                if (charts[name]) {
                    updateChart(name, value);
                } else if (shouldCreateChart(name, value)) {
                    createChart(name, value);
                    updateChart(name, value);
                }
            }
        }

        function updateVehicleInfo(name, value) {
            const infoGrid = document.querySelector('.info-grid');
            let infoItem = document.getElementById(`info-${name}`);
            
            // Group information by category
            const categories = {
                'Basic Information': ['VIN', 'MANUFACTURER', 'MODEL', 'YEAR', 'BODY_STYLE', 'DOORS'],
                'Engine & Performance': ['ENGINE_INFO', 'FUEL_TYPE', 'DRIVE_TYPE'],
                'Manufacturing': ['MANUFACTURER_NAME', 'MANUFACTURER_ID', 'PLANT_LOCATION'],
                'Technical Details': ['VEHICLE_TYPE', 'SERIES', 'GVWR'],
                'Standards & Compliance': [
                    'OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO',
                    'EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED',
                    'FUEL_TYPE'
                ],
                'Diagnostic Information': [
                    'TIME_SINCE_TROUBLE_CODES_CLEARED',
                    'WARM_UPS_SINCE_CODES_CLEARED',
                    'DISTANCE_TRAVELED_SINCE_CODES_CLEARED',
                    'MONITOR_STATUS_SINCE_DTCS_CLEARED',
                    'MONITOR_STATUS_THIS_DRIVE_CYCLE',
                    'TIME_RUN_WITH_MIL_ON'
                ]
            };

            // Create or update sections
            for (const [category, items] of Object.entries(categories)) {
                let section = document.getElementById(`section-${category.replace(/\s+/g, '-')}`);
                if (!section && items.includes(name)) {
                    section = document.createElement('div');
                    section.id = `section-${category.replace(/\s+/g, '-')}`;
                    section.className = 'info-section';
                    section.innerHTML = `<h4>${category}</h4>`;
                    
                    // Find the right position to insert the section
                    const categoryIndex = Object.keys(categories).indexOf(category);
                    const existingSections = infoGrid.querySelectorAll('.info-section');
                    let insertBeforeElement = null;
                    
                    for (let i = categoryIndex + 1; i < Object.keys(categories).length; i++) {
                        const nextSection = document.getElementById(`section-${Object.keys(categories)[i].replace(/\s+/g, '-')}`);
                        if (nextSection) {
                            insertBeforeElement = nextSection;
                            break;
                        }
                    }
                    
                    if (insertBeforeElement) {
                        infoGrid.insertBefore(section, insertBeforeElement);
                    } else {
                        infoGrid.appendChild(section);
                    }
                }

                // If this is the right section for our item
                if (section && items.includes(name)) {
                    if (!infoItem) {
                        infoItem = document.createElement('div');
                        infoItem.id = `info-${name}`;
                        infoItem.className = 'info-item';
                        infoItem.innerHTML = `
                            ${getStatusIndicator(value)}
                            <div class="label">${formatLabel(name)}</div>
                            <div class="value"></div>
                        `;
                        section.appendChild(infoItem);
                    }

                    const valueElement = infoItem.querySelector('.value');
                    if (valueElement.innerHTML !== value) {
                        valueElement.innerHTML = value;
                        infoItem.classList.remove('value-update');
                        void infoItem.offsetWidth;
                        infoItem.classList.add('value-update');
                    }
                }
            }
        }

        function formatLabel(name) {
            // Special cases for better readability
            const specialCases = {
                'TIME_SINCE_TROUBLE_CODES_CLEARED': 'Time Since Codes Cleared',
                'WARM_UPS_SINCE_CODES_CLEARED': 'Warm-ups Since Codes Cleared',
                'DISTANCE_TRAVELED_SINCE_CODES_CLEARED': 'Distance Since Codes Cleared',
                'MONITOR_STATUS_SINCE_DTCS_CLEARED': 'Monitor Status Since DTCs Cleared',
                'MONITOR_STATUS_THIS_DRIVE_CYCLE': 'Monitor Status This Drive',
                'TIME_RUN_WITH_MIL_ON': 'Time with MIL On',
                'OBD_STANDARDS_THIS_VEHICLE_CONFORMS_TO': 'OBD Standards',
                'EMISSION_REQUIREMENT_TO_WHICH_VEHICLE_IS_DESIGNED': 'Emission Requirements'
            };

            return specialCases[name] || name.replace(/_/g, ' ');
        }

        function filterData() {
            const searchTerm = document.getElementById('search-bar').value.toLowerCase();

            document.querySelectorAll('.info-item').forEach(item => {
                const label = item.querySelector('.label').textContent.toLowerCase();
                const value = item.querySelector('.value').textContent.toLowerCase();
                item.style.display = 
                    label.includes(searchTerm) || value.includes(searchTerm) 
                    ? '' 
                    : 'none';
            });
            
            const rows = dataBody.getElementsByTagName('tr');
            Array.from(rows).forEach(row => {
                const name = row.children[0].textContent.toLowerCase();
                const value = row.children[1].textContent.toLowerCase();
                row.style.display = 
                    name.includes(searchTerm) || value.includes(searchTerm) 
                    ? '' 
                    : 'none';
            });

            document.querySelectorAll('.chart-container').forEach(container => {
                const chartId = container.querySelector('canvas').id;
                const chartName = chartId.replace('chart-', '').toLowerCase();
                container.style.display = 
                    chartName.includes(searchTerm) 
                    ? '' 
                    : 'none';
            });
        }

        async function decodeVIN(vin) {
            try {
                // Build the API URL
                const apiUrl = new URL('https://vpic.nhtsa.dot.gov/api/vehicles/DecodeVinValues/' + vin);
                apiUrl.searchParams.append('format', 'json');

                const response = await fetch(apiUrl.toString());
                if (!response.ok) {
                    throw new Error('Failed to decode VIN');
                }

                const data = await response.json();
                if (!data.Results || data.Results.length === 0) {
                    throw new Error('Invalid VIN data');
                }

                const result = data.Results[0];
                return {
                    manufacturer: result.Make || 'Unknown',
                    model: result.Model || 'Unknown',
                    year: result.ModelYear || 'Unknown',
                    bodyClass: result.BodyClass || 'Unknown',
                    engine: formatEngineInfo({
                        'Engine Number of Cylinders': result.EngineNumberofCylinders,
                        'Displacement (L)': result.DisplacementL,
                        'Engine Model': result.EngineModel
                    }),
                    fuelType: result.FuelTypePrimary || 'Unknown',
                    transmission: result.TransmissionStyle || 'Unknown',
                    driveType: result.DriveType || 'Unknown',
                    cylinders: result.EngineNumberofCylinders || 'Unknown',
                    displacement: result.DisplacementL || 'Unknown',
                    plantCountry: result.PlantCountry || 'Unknown',
                    plantState: result.PlantState || 'Unknown',
                    vehicleType: result.VehicleType || 'Unknown',
                    manufacturerCode: result.ManufacturerId || 'Unknown',
                    engineHP: result.EngineHP || 'Unknown',
                    engineKW: result.EngineKW || 'Unknown',
                    engineManufacturer: result.EngineManufacturer || 'Unknown',
                    series: result.Series || 'Unknown',
                    trim: result.Trim || 'Unknown'
                };
            } catch (error) {
                console.error('Error decoding VIN:', error);
                return getDefaultVINInfo(vin);
            }
        }

        function getModelYearFromVIN(vin) {
            if (vin.length < 10) return null;
            
            const yearChar = vin.charAt(9);
            const yearMap = {
                'A': '2010', 'B': '2011', 'C': '2012', 'D': '2013', 'E': '2014',
                'F': '2015', 'G': '2016', 'H': '2017', 'J': '2018', 'K': '2019',
                'L': '2020', 'M': '2021', 'N': '2022', 'P': '2023', 'R': '2024'
            };
            
            return yearMap[yearChar] || null;
        }

        function parseVINJSONResponse(data) {
            if (!data.Results || data.Results.length === 0) {
                throw new Error('Invalid VIN data');
            }

            const results = data.Results.reduce((acc, item) => {
                if (item.Value && item.Value !== 'Not Applicable' && item.Value !== 'null') {
                    acc[item.Variable] = item.Value;
                }
                return acc;
            }, {});

            return {
                manufacturer: results['Make'] || 'Unknown',
                model: results['Model'] || 'Unknown',
                year: results['Model Year'] || 'Unknown',
                bodyClass: results['Body Class'] || 'Unknown',
                engine: formatEngineInfo(results),
                fuelType: results['Fuel Type - Primary'] || 'Unknown',
                transmission: results['Transmission Style'] || 'Unknown',
                driveType: results['Drive Type'] || 'Unknown',
                cylinders: results['Engine Number of Cylinders'] || 'Unknown',
                displacement: results['Displacement (L)'] || 'Unknown',
                plantCountry: results['Plant Country'] || 'Unknown',
                plantState: results['Plant State'] || 'Unknown',
                vehicleType: results['Vehicle Type'] || 'Unknown',
                manufacturerCode: results['Manufacturer Id'] || 'Unknown',
                gvwr: results['GVWR'] || 'Unknown',
                abs: results['Anti-Lock Braking System (ABS)'] || 'Unknown',
                engineHP: results['Engine Horse Power'] || 'Unknown',
                engineKW: results['Engine Power (kW)'] || 'Unknown',
                engineManufacturer: results['Engine Manufacturer'] || 'Unknown',
                steeringLocation: results['Steering Location'] || 'Unknown',
                series: results['Series'] || 'Unknown',
                trim: results['Trim'] || 'Unknown'
            };
        }

        function parseVINXMLResponse(xmlText) {
            const parser = new DOMParser();
            const xmlDoc = parser.parseFromString(xmlText, 'text/xml');
            const results = xmlDoc.getElementsByTagName('Result');
            
            const data = {};
            for (let result of results) {
                const variable = result.getElementsByTagName('Variable')[0]?.textContent;
                const value = result.getElementsByTagName('Value')[0]?.textContent;
                if (value && value !== 'Not Applicable' && value !== 'null') {
                    data[variable] = value;
                }
            }
            
            return parseVINJSONResponse({ Results: Object.entries(data).map(([k, v]) => ({ Variable: k, Value: v })) });
        }

        function formatEngineInfo(results) {
            const engineParts = [];
            if (results['Engine Number of Cylinders']) {
                engineParts.push(`${results['Engine Number of Cylinders']} cyl`);
            }
            if (results['Displacement (L)']) {
                engineParts.push(`${results['Displacement (L)']}L`);
            }
            if (results['Engine Model']) {
                engineParts.push(results['Engine Model']);
            }
            return engineParts.length > 0 ? engineParts.join(' ') : 'Unknown';
        }

        function getDefaultVINInfo(vin) {
            return {
                manufacturer: 'Not Available',
                model: 'Not Available',
                year: 'Not Available',
                bodyClass: 'Not Available',
                engine: 'Not Available',
                fuelType: 'Not Available',
                transmission: 'Not Available',
                driveType: 'Not Available',
                cylinders: 'Not Available',
                displacement: 'Not Available',
                plantCountry: 'Not Available',
                plantState: 'Not Available',
                vehicleType: 'Not Available',
                manufacturerCode: 'Not Available',
                gvwr: 'Not Available',
                abs: 'Not Available',
                engineHP: 'Not Available',
                engineKW: 'Not Available',
                engineManufacturer: 'Not Available',
                steeringLocation: 'Not Available',
                series: 'Not Available',
                trim: 'Not Available'
            };
        }

        function updateVINDetails(vin) {
            const vinDetails = document.getElementById('vin-details');
            vinDetails.innerHTML = '<p>Loading vehicle information...</p>';

            decodeVIN(vin).then(vinInfo => {
                // Format the display of unknown values
                const formatValue = (value) => value === 'Unknown' || value === 'null' ? 'Not Available' : value;
                
                vinDetails.innerHTML = `
                    <div class="vin-info-section">
                        <h4>Basic Information</h4>
                        <p><span class="label">Manufacturer:</span> <span class="value">${formatValue(vinInfo.manufacturer)}</span></p>
                        <p><span class="label">Model:</span> <span class="value">${formatValue(vinInfo.model)}</span></p>
                        <p><span class="label">Year:</span> <span class="value">${formatValue(vinInfo.year)}</span></p>
                        ${vinInfo.series !== 'Unknown' ? `<p><span class="label">Series:</span> <span class="value">${formatValue(vinInfo.series)}</span></p>` : ''}
                        ${vinInfo.trim !== 'Unknown' ? `<p><span class="label">Trim:</span> <span class="value">${formatValue(vinInfo.trim)}</span></p>` : ''}
                    </div>
                    ${vinInfo.engine !== 'Unknown' || vinInfo.fuelType !== 'Unknown' ? `
                    <div class="vin-info-section">
                        <h4>Engine & Performance</h4>
                        ${vinInfo.engine !== 'Unknown' ? `<p><span class="label">Engine:</span> <span class="value">${formatValue(vinInfo.engine)}</span></p>` : ''}
                        ${vinInfo.engineHP !== 'Unknown' && vinInfo.engineKW !== 'Unknown' ? `<p><span class="label">Power:</span> <span class="value">${formatValue(vinInfo.engineHP)}hp / ${formatValue(vinInfo.engineKW)}kW</span></p>` : ''}
                        ${vinInfo.fuelType !== 'Unknown' ? `<p><span class="label">Fuel Type:</span> <span class="value">${formatValue(vinInfo.fuelType)}</span></p>` : ''}
                        ${vinInfo.transmission !== 'Unknown' ? `<p><span class="label">Transmission:</span> <span class="value">${formatValue(vinInfo.transmission)}</span></p>` : ''}
                        ${vinInfo.driveType !== 'Unknown' ? `<p><span class="label">Drive Type:</span> <span class="value">${formatValue(vinInfo.driveType)}</span></p>` : ''}
                    </div>
                    ` : ''}
                    ${vinInfo.bodyClass !== 'Unknown' || vinInfo.vehicleType !== 'Unknown' ? `
                    <div class="vin-info-section">
                        <h4>Vehicle Details</h4>
                        ${vinInfo.bodyClass !== 'Unknown' ? `<p><span class="label">Body Type:</span> <span class="value">${formatValue(vinInfo.bodyClass)}</span></p>` : ''}
                        ${vinInfo.vehicleType !== 'Unknown' ? `<p><span class="label">Vehicle Type:</span> <span class="value">${formatValue(vinInfo.vehicleType)}</span></p>` : ''}
                    </div>
                    ` : ''}
                    <div class="vin-info-section">
                        <h4>Manufacturing</h4>
                        ${vinInfo.plantCountry !== 'Unknown' ? `<p><span class="label">Plant Location:</span> <span class="value">${formatValue(vinInfo.plantCountry)}${vinInfo.plantState ? ', ' + vinInfo.plantState : ''}</span></p>` : ''}
                        ${vinInfo.manufacturerCode !== 'Unknown' ? `<p><span class="label">Manufacturer Code:</span> <span class="value">${formatValue(vinInfo.manufacturerCode)}</span></p>` : ''}
                        <p><span class="label">VIN:</span> <span class="value">${vin}</span></p>
                    </div>
                `;
            });
        }

        function getStatusIndicator(value) {
            if (value.includes('error') || value.includes('Not available')) {
                return '<span class="status-indicator status-error"></span>';
            }
            if (value.includes('warning')) {
                return '<span class="status-indicator status-warning"></span>';
            }
            return '<span class="status-indicator status-good"></span>';
        }

        function updateChart(name, value) {
            const chart = charts[name];
            if (!chart) return;

            const numericValue = extractNumericValue(value);
            if (numericValue === null) return;

            const currentTime = new Date().toLocaleTimeString();

            requestAnimationFrame(() => {
                chart.data.labels.push(currentTime);
                chart.data.datasets[0].data.push(numericValue);

                if (chart.data.labels.length > config.maxDataPoints) {
                    chart.data.labels.shift();
                    chart.data.datasets[0].data.shift();
                }

                chart.update('none');
            });
        }
  