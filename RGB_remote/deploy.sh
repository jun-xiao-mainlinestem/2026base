#!/bin/bash

# RGB Remote Control Deployment Script

echo "🚀 Building RGB Remote Control application..."

# Build the application
npm run build

if [ $? -eq 0 ]; then
    echo "✅ Build successful!"
    echo "📁 Production files created in 'dist/' folder"
    echo ""
    echo "🌐 To deploy to Netlify:"
    echo "   1. Drag and drop the 'dist/' folder to Netlify"
    echo "   2. Or connect your GitHub repo and set:"
    echo "      - Build command: npm run build"
    echo "      - Publish directory: dist"
    echo ""
    echo "📱 To test locally:"
    echo "   npm run dev"
    echo ""
    echo "📦 Files ready for deployment:"
    ls -la dist/
else
    echo "❌ Build failed!"
    exit 1
fi
